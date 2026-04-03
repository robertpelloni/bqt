package kernel

import (
	"fmt"
	"log"
	"runtime/debug"
)

type SelfHealer struct {
	Active bool
}

func GetSelfHealer() *SelfHealer { return &SelfHealer{Active: true} }

func (sh *SelfHealer) Intercept(taskName string, aiCallback func(string)) {
	if !sh.Active { return }
	
	if r := recover(); r != nil {
		stack := debug.Stack()
		report := fmt.Sprintf("TASK: %s\nERROR: %v\nSTACK:\n%s", taskName, r, string(stack))
		log.Printf("OMNI HEALER: Caught Critical Panic. Dispatching to AI Assistant...")
		
		// Physically pipe the crash to the AI Co-Processor
		aiCallback(report)
	}
}
