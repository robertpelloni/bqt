package vm

import (
	"log"
	"sync"
)

type AISuggestion struct {
	Text       string  `json:"text"`
	Confidence float32 `json:"confidence"`
}

type AIAssistant struct {
	mu             sync.Mutex
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

// RequestCompletion aggregates the locally available context and hands it off to
// an external LLM bridge. In the verified Go baseline this function is intentionally
// side-effect-light and avoids cyclic dependencies.
func (aa *AIAssistant) RequestCompletion(codeContext string) {
	ne := GetNeuralEngine()
	dom, _ := ne.CaptureDOM()
	_ = dom
	log.Printf("OMNIAI Go: Dispatching cognitive context to external LLM bridge (%d chars)", len(codeContext))
}

func (aa *AIAssistant) GetLatestSuggestion() AISuggestion {
	aa.mu.Lock()
	defer aa.mu.Unlock()
	return aa.LastSuggestion
}

func (aa *AIAssistant) SetLatestSuggestion(s AISuggestion) {
	aa.mu.Lock()
	defer aa.mu.Unlock()
	aa.LastSuggestion = s
}
