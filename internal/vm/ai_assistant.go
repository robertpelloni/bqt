package vm

import (
	"log"
	"sync"
	"github.com/robertpelloni/bobui/internal/kernel"
)

type AISuggestion struct {
	Text string `json:"text"`
	Confidence float32 `json:"confidence"`
}

type AIAssistant struct {
	mu sync.Mutex
	LastSuggestion AISuggestion
}

var (
	assistantInstance *AIAssistant
	assistantOnce     sync.Once
)

func GetAIAssistant() *AIAssistant {
	assistantOnce.Do(func() {
		assistantInstance = &AIAssistant{}
	})
	return assistantInstance
}

// RequestCompletion sends the system context to the LLM for code assistance.
func (aa *AIAssistant) RequestCompletion(codeContext string) {
	ne := GetNeuralEngine()
	dom, _ := ne.CaptureDOM()

	// Aggregated AI Context
	context := map[string]interface{}{
		"code": codeContext,
		"visual_dom": dom,
	}

	log.Printf("OMNIAI Go: Dispatching Cognitive Context to LLM (%d chars)", len(codeContext))
	
	// In a full implementation, we'd fire an async POST to a local LLM server
	// and update LastSuggestion on response.
	_ = context
}

func (aa *AIAssistant) GetLatestSuggestion() AISuggestion {
	aa.mu.Lock()
	defer aa.mu.Unlock()
	return aa.LastSuggestion
}
