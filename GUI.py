import gradio as gr
from subprocess import PIPE, Popen
import traceback
import os
import time

case_root = "./case/"
cache_root = "./cache/"
output_text = ""
cmd_cnt = 0
process = None

def wait():
    global cmd_cnt
    while not os.path.exists(os.path.join(cache_root, f"{cmd_cnt}.txt")): pass
    cmd_cnt+=1
    return cmd_cnt-1

def load_file(file_path):
    if file_path is None:
        return "please choose a file first"
    else:
        f = open(file_path, "r")
        return f.read()
def save_file(source, file_name, file):
    if file_name is not None:
        f = open(os.path.join(case_root, file_name), "w")
        f.write(source)

def format_reg_info(raw_str):
    regs = str.split(raw_str, "\n")
    reg_dist = {
        'pc': None,
        'code_cnt': None,
        'pstk': None,
        'base': None,
    }
    for reg in regs:
        if(reg == ""): continue
        reg_info = str.split(reg, ":")
        if len(reg_info) == 2: reg_dist[reg_info[0]] = reg_info[1]
    reg_pc = f"**pc**: {reg_dist.get('pc')}/{reg_dist.get('code_cnt')}\n\n"
    reg_pstk = f"**current stack**: {reg_dist.get('pstk')}\n\n"
    reg_base = f"**base**: {reg_dist.get('base')}"
    md = reg_pc + reg_pstk + reg_base
    return md
def update_output(text):
    global output_text
    output_text += text
    return output_text
def proc_communicate(proc, text):
    proc.stdin.write(text)
    proc.stdin.flush()
    ret = ""
    id = wait()
    with open(os.path.join(cache_root, f"{id}.txt"),"r") as f:
        ret = f.read()
    return ret

def fresh():
    return "", "", "", "", format_reg_info("")

def execute():
    output = proc_communicate(process, "e")
    stack0 = proc_communicate(process, "1")
    stack1 = proc_communicate(process, "2")
    strarr = proc_communicate(process, "3")
    regs = proc_communicate(process, "4")
    return stack0, stack1, strarr, update_output(output), format_reg_info(regs)
def step():
    output = proc_communicate(process, "s")
    stack0 = proc_communicate(process, "1")
    stack1 = proc_communicate(process, "2")
    strarr = proc_communicate(process, "3")
    regs = proc_communicate(process, "4")
    return stack0, stack1, strarr, update_output(output), format_reg_info(regs)

def run_cmd(source_code, input_data):
    global process, cmd_cnt, output_text
    with open(os.path.join("./fsource.txt"), "w") as f:
        f.write(source_code)
        f.flush()
    with open(os.path.join("./finput.txt"), "w") as f:
        f.write(input_data)
        f.flush()
    if process is not None and process.poll() is not None:
            process.terminate()
            process = None
    if os.path.exists(cache_root): 
        for file in os.listdir(cache_root): os.remove(os.path.join(cache_root, file))
    else: os.mkdir(cache_root)

    exe = "L24Parser.exe"
    cmd = f"{exe} ./fsource.txt -g"
    log = ""
    ins = ""
    try:
        process = Popen(cmd, shell=True, stdin=PIPE, stdout=PIPE, text=True)
        cmd_cnt = 0
        output_text = ""
        wait()
        with open(os.path.join("./flog.txt"), "r") as flog:
            log = flog.read()
        with open(os.path.join("./fcode.txt"), "r") as fcode:
            ins = fcode.read()
        os.remove(os.path.join("./fsource.txt"))
    except Exception as e:
        print("WRONG")
        output = str(e)
        print(output)
        stack_info = traceback.format_exc()
        print(stack_info)
    
    return ins, log

with gr.Blocks() as demo:
    gr.Markdown("L24 language")
    with gr.Row():
        with gr.Column(scale = 1):
            with gr.Tab("test case"):
                file = gr.FileExplorer(glob = "*.txt", file_count='single', root_dir=case_root, label="Select soruce code")
                load_button = gr.Button("load", size="sm")
            with gr.Tab("source code"):
                soruce_code = gr.TextArea(interactive=True, placeholder="Enter command here...", show_label=False)
                save_name = gr.Textbox(label="saved file name", placeholder="a.txt")
                save_button = gr.Button("save source code", size="sm")
                exe_input = gr.TextArea(lines=5, label="input")
                compile_button = gr.Button("save input and compile", size="sm")
            with gr.Tab("instruction"):
                instruction = gr.TextArea(lines = 15, interactive=False, show_label=False)
                with gr.Row():
                    excute_button = gr.Button("excute", size="sm")
                    step_button = gr.Button("step", size="sm")
        with gr.Column(scale = 2):
            with gr.Tab("compiler"):
                log = gr.TextArea(label="log")
            with gr.Tab("interpreter"):
                with gr.Column():
                    with gr.Row():
                        stack0 = gr.TextArea(lines = 15, label="Stack 0")
                        stack1 = gr.TextArea(lines = 15, label="Stack 1")
                        strarr = gr.TextArea(lines = 15, label="Str array")
                with gr.Column():
                    with gr.Row():
                        reg_info = gr.Markdown(format_reg_info(""))
                        exe_output = gr.TextArea(label="output")
    instruction.change(fresh, outputs=[stack0, stack1, strarr, exe_output, reg_info])
    save_button.click(save_file, inputs=[soruce_code, save_name, file], outputs=None)
    load_button.click(load_file, inputs=[file], outputs=soruce_code)
    compile_button.click(run_cmd, inputs=[soruce_code, exe_input], outputs=[instruction, log])
    excute_button.click(execute, outputs=[stack0, stack1, strarr, exe_output, reg_info])
    step_button.click(step, outputs=[stack0, stack1, strarr, exe_output, reg_info])

if __name__ == "__main__":
    demo.launch(share=True)