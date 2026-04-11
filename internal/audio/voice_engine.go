package audio

import (
	"log"
	"sync"
)

type VoiceEngine struct {
	mu          sync.Mutex
	IsListening bool
	audioBuffer []byte
}

var (
	voiceInstance *VoiceEngine
	voiceOnce     sync.Once
)

func GetVoiceEngine() *VoiceEngine {
	voiceOnce.Do(func() {
		voiceInstance = &VoiceEngine{}
	})
	return voiceInstance
}

// StartListening activates the Go-native microphone capture loop.
func (ve *VoiceEngine) StartListening() {
	ve.mu.Lock()
	defer ve.mu.Unlock()
	ve.IsListening = true
	ve.audioBuffer = make([]byte, 0)
	log.Println("OmniVoice Go: Started native hardware PCM capture.")
}

// StopListening flushes the Go buffer and prepares for STT dispatch.
func (ve *VoiceEngine) StopListening() []byte {
	ve.mu.Lock()
	defer ve.mu.Unlock()
	ve.IsListening = false
	log.Printf("OmniVoice Go: Captured %d bytes of PCM data.", len(ve.audioBuffer))
	return ve.audioBuffer
}

// PushData is called by the hardware-interrupt goroutine.
func (ve *VoiceEngine) PushData(data []byte) {
	ve.mu.Lock()
	defer ve.mu.Unlock()
	if ve.IsListening {
		ve.audioBuffer = append(ve.audioBuffer, data...)
	}
}
