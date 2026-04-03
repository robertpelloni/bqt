package kernel

import (
	"bufio"
	"io"
	"os/exec"
	"runtime"
)

type ShellProcess struct {
	cmd    *exec.Cmd
	stdin  io.WriteCloser
	stdout io.ReadCloser
	Output chan string
}

func NewShell() (*ShellProcess, error) {
	shell := "bash"
	if runtime.GOOS == "windows" {
		shell = "cmd.exe"
	}

	cmd := exec.Command(shell)
	stdin, _ := cmd.StdinPipe()
	stdout, _ := cmd.StdoutPipe()
	
	sp := &ShellProcess{
		cmd:    cmd,
		stdin:  stdin,
		stdout: stdout,
		Output: make(chan string, 100),
	}

	if err := cmd.Start(); err != nil {
		return nil, err
	}

	// Start Async Pipe Drainer
	go sp.drain()

	return sp, nil
}

func (sp *ShellProcess) drain() {
	scanner := bufio.NewScanner(sp.stdout)
	for scanner.Scan() {
		sp.Output <- scanner.Text()
	}
}

func (sp *ShellProcess) Write(input string) {
	io.WriteString(sp.stdin, input+"\n")
}
