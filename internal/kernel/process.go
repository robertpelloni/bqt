package kernel

import (
	"bufio"
	"io"
	"os"
	"os/exec"
	"runtime"
	"sync"
)

type ProcessState int

const (
	NotRunning ProcessState = iota
	Starting
	Running
)

type ProcessError int

const (
	FailedToStart ProcessError = iota
	Crashed
	Timedout
	ReadError
	WriteError
	UnknownError
)

type Process struct {
	mu sync.RWMutex

	Program          string
	Arguments        []string
	WorkingDirectory string
	Environment      []string

	cmd    *exec.Cmd
	stdin  io.WriteCloser
	stdout io.ReadCloser
	stderr io.ReadCloser

	state ProcessState
	err   ProcessError

	Output chan string
	Error  chan string
	Done   chan error
}

func NewProcess() *Process {
	return &Process{
		Output: make(chan string, 100),
		Error:  make(chan string, 100),
		Done:   make(chan error, 1),
		state:  NotRunning,
	}
}

func NewShell() (*Process, error) {
	shell := "bash"
	if runtime.GOOS == "windows" {
		shell = "cmd.exe"
	}

	p := NewProcess()
	p.Program = shell
	if err := p.Start(); err != nil {
		return nil, err
	}
	return p, nil
}

func (p *Process) Start() error {
	p.mu.Lock()
	defer p.mu.Unlock()

	p.state = Starting
	p.cmd = exec.Command(p.Program, p.Arguments...)
	p.cmd.Dir = p.WorkingDirectory
	if len(p.Environment) > 0 {
		p.cmd.Env = append(os.Environ(), p.Environment...)
	}

	stdin, err := p.cmd.StdinPipe()
	if err != nil {
		p.state = NotRunning
		return err
	}
	p.stdin = stdin

	stdout, err := p.cmd.StdoutPipe()
	if err != nil {
		p.state = NotRunning
		return err
	}
	p.stdout = stdout

	stderr, err := p.cmd.StderrPipe()
	if err != nil {
		p.state = NotRunning
		return err
	}
	p.stderr = stderr

	if err := p.cmd.Start(); err != nil {
		p.state = NotRunning
		return err
	}

	p.state = Running
	go p.drain(p.stdout, p.Output)
	go p.drain(p.stderr, p.Error)
	go func() {
		err := p.cmd.Wait()
		p.mu.Lock()
		p.state = NotRunning
		p.mu.Unlock()
		p.Done <- err
	}()

	return nil
}

func (p *Process) drain(r io.ReadCloser, c chan string) {
	scanner := bufio.NewScanner(r)
	for scanner.Scan() {
		c <- scanner.Text()
	}
}

func (p *Process) Write(input string) (int, error) {
	p.mu.RLock()
	defer p.mu.RUnlock()
	if p.state != Running {
		return 0, io.ErrClosedPipe
	}
	return io.WriteString(p.stdin, input+"\n")
}

func (p *Process) State() ProcessState {
	p.mu.RLock()
	defer p.mu.RUnlock()
	return p.state
}

func (p *Process) Kill() error {
	p.mu.Lock()
	defer p.mu.Unlock()
	if p.cmd != nil && p.cmd.Process != nil {
		return p.cmd.Process.Kill()
	}
	return nil
}

func (p *Process) Terminate() error {
	p.mu.Lock()
	defer p.mu.Unlock()
	if p.cmd != nil && p.cmd.Process != nil {
		return p.cmd.Process.Signal(os.Interrupt)
	}
	return nil
}
