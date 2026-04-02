import os
import glob

def resolve_file(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()
        
    resolved_lines = []
    in_conflict = False
    
    for line in lines:
        if line.startswith('<<<<<<< HEAD'):
            in_conflict = True
            continue
        elif line.startswith('======='):
            continue
        elif line.startswith('>>>>>>>'):
            in_conflict = False
            continue
            
        resolved_lines.append(line)
        
    with open(filepath, 'w', encoding='utf-8') as f:
        f.writelines(resolved_lines)

# Find all conflicted files
import subprocess
result = subprocess.run(['git', 'diff', '--name-only', '--diff-filter=U'], capture_output=True, text=True, cwd='/c/Users/hyper/workspace/bobui')
conflicted_files = result.stdout.strip().split('\n')

for file in conflicted_files:
    if file:
        abs_path = os.path.join('/c/Users/hyper/workspace/bobui', file)
        resolve_file(abs_path)
        print(f"Resolved {file} by combining both additions.")

