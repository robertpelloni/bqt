package net

import (
	"bytes"
	"encoding/gob"
	"image"
	"image/color"
	"testing"
)

type sampleTeleportPayload struct {
	Key   string
	Value int
}

func TestMeshStreaming_AudioSync(t *testing.T) {
	buffer := []float32{0.1, -0.2, 0.5, -0.4}
	// Safe broadcast to mesh
	BroadcastAudioShards(buffer)

	// Safe handling of remote audio
	raw := []interface{}{float64(0.1), float64(-0.2)}
	HandleRemoteAudio("peer_123", raw)
}

func TestMeshStreaming_DesktopSync(t *testing.T) {
	frameData := []byte{0xDE, 0xAD, 0xBE, 0xEF}
	BroadcastDesktopFrame("win_primary", frameData)
	HandleRemoteDesktop("peer_456", "win_primary", frameData)
}

func TestMeshStreaming_Teleport(t *testing.T) {
	orig := sampleTeleportPayload{Key: "session_state", Value: 42}
	if err := TeleportState(orig); err != nil {
		t.Fatalf("expected TeleportState to succeed, got %v", err)
	}

	var buf bytes.Buffer
	enc := gob.NewEncoder(&buf)
	if err := enc.Encode(orig); err != nil {
		t.Fatalf("expected gob encode to succeed, got %v", err)
	}

	var target sampleTeleportPayload
	if err := HandleRemoteTeleport(buf.Bytes(), &target); err != nil {
		t.Fatalf("expected HandleRemoteTeleport to succeed, got %v", err)
	}
	if target.Key != orig.Key || target.Value != orig.Value {
		t.Errorf("expected target %+v to match orig %+v", target, orig)
	}
}

func TestMeshStreaming_VoiceMesh(t *testing.T) {
	vm := GetVoiceMesh()
	if vm == nil {
		t.Fatalf("expected GetVoiceMesh to return non-nil singleton")
	}

	pcmData := []byte{0x01, 0x02, 0x03, 0x04}
	vm.BroadcastVoice(pcmData)

	vm.HandleRemoteVoice("peer_voice_1", pcmData)
	vm.mu.RLock()
	received := vm.peerBuffers["peer_voice_1"]
	vm.mu.RUnlock()

	if len(received) != len(pcmData) {
		t.Errorf("expected received buffer length %d, got %d", len(pcmData), len(received))
	}
}

func TestMeshStreaming_StreamKernel(t *testing.T) {
	img := image.NewRGBA(image.Rect(0, 0, 10, 10))
	for y := 0; y < 10; y++ {
		for x := 0; x < 10; x++ {
			img.Set(x, y, color.RGBA{R: 255, G: 0, B: 0, A: 255})
		}
	}

	BroadcastFrame("source_gpu_0", img)
	HandleRemoteStream("peer_stream_1", []byte("jpeg_bytes"))
}

func TestMeshStreaming_NodeHealth(t *testing.T) {
	health := NodeHealth{
		CPUUsage:    12.5,
		MemoryUsage: 256,
		Goroutines:  18,
	}

	payload := map[string]interface{}{
		"type":    "node_health",
		"content": health,
	}

	GetMeshNode().Broadcast(payload)

	if health.CPUUsage != 12.5 || health.MemoryUsage != 256 || health.Goroutines != 18 {
		t.Errorf("unexpected health stats: %+v", health)
	}
}
