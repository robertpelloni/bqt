package vm

import (
	"fmt"
	"strings"
	"sync"
)

type MacroRecorder struct {
	mu           sync.Mutex
	IsRecording  bool
	IsPlaying    bool
	CommandLog   []string
}

var (
	macroInstance *MacroRecorder
	macroOnce     sync.Once
)

func GetMacroRecorder() *MacroRecorder {
	macroOnce.Do(func() {
		macroInstance = &MacroRecorder{
			CommandLog: make([]string, 0),
		}
	})
	return macroInstance
}

// RecordClick synthesizes a Lisp AST command for a mouse interaction.
func (mr *MacroRecorder) RecordClick(target string) {
	mr.mu.Lock()
	defer mr.mu.Unlock()
	if mr.IsRecording {
		mr.CommandLog = append(mr.CommandLog, fmt.Sprintf("(click \"%s\")", target))
	}
}

// GenerateScript flattens the Go buffer into a pure OmniScript payload.
func (mr *MacroRecorder) GenerateScript() string {
	mr.mu.Lock()
	defer mr.mu.Unlock()
	return strings.Join(mr.CommandLog, " ")
}

func (mr *MacroRecorder) Start() {
	mr.mu.Lock()
	defer mr.mu.Unlock()
	mr.CommandLog = make([]string, 0)
	mr.IsRecording = true
}

func (mr *MacroRecorder) Stop() {
	mr.mu.Lock()
	defer mr.mu.Unlock()
	mr.IsRecording = false
}
