package audio

import (
	"bytes"
	"testing"
)

func TestVoiceEngineLifecycle(t *testing.T) {
	ve := GetVoiceEngine()
	if ve == nil {
		t.Fatal("Expected GetVoiceEngine to return non-nil instance")
	}

	// Verify initial start listening
	ve.StartListening()
	if !ve.IsListening {
		t.Errorf("Expected IsListening to be true after StartListening")
	}

	// Push sample PCM data
	sampleData := []byte{0x01, 0x02, 0x03, 0x04}
	ve.PushData(sampleData)

	// Stop listening and check returned buffer
	captured := ve.StopListening()
	if ve.IsListening {
		t.Errorf("Expected IsListening to be false after StopListening")
	}
	if !bytes.Equal(captured, sampleData) {
		t.Errorf("Expected captured buffer %v, got %v", sampleData, captured)
	}

	// Data pushed after stop listening should be ignored
	ve.PushData([]byte{0x99})
	capturedAfterStop := ve.StopListening()
	if len(capturedAfterStop) != 0 {
		t.Errorf("Expected 0 bytes captured while not listening, got %d", len(capturedAfterStop))
	}
}
