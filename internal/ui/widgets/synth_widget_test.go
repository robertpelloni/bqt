package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
)

func TestSynthWidgetLayout(t *testing.T) {
	widget := NewSynthWidget()

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops: ops,
		Constraints: layout.Constraints{
			Max: image.Pt(800, 600),
		},
	}

	buffer := make([]float32, 256)
	for i := range buffer {
		buffer[i] = 0.5
	}
	widget.UpdateWaveform(buffer)

	dims := widget.Layout(gtx)

	if dims.Size.X != 800 || dims.Size.Y != 600 {
		t.Errorf("Expected dimensions (800, 600), got (%d, %d)", dims.Size.X, dims.Size.Y)
	}
}

func TestSynthWidgetBufferAndZeroConstraints(t *testing.T) {
	widget := NewSynthWidget()

	// 1. Set active voices
	widget.SetActiveVoices(16)
	if widget.activeVoices != 16 {
		t.Fatalf("expected activeVoices 16, got %d", widget.activeVoices)
	}

	// 2. Buffer longer than internal slice (>256)
	largeBuffer := make([]float32, 512)
	for i := range largeBuffer {
		largeBuffer[i] = 0.75
	}
	widget.UpdateWaveform(largeBuffer)
	if widget.waveformData[0] != 0.75 {
		t.Fatalf("expected sample 0.75, got %f", widget.waveformData[0])
	}

	// 3. Buffer shorter than internal slice (<256)
	shortBuffer := make([]float32, 64)
	for i := range shortBuffer {
		shortBuffer[i] = -0.25
	}
	widget.UpdateWaveform(shortBuffer)
	if widget.waveformData[0] != -0.25 {
		t.Fatalf("expected sample -0.25, got %f", widget.waveformData[0])
	}

	// 4. Zero constraints layout
	ops := new(op.Ops)
	gtxZero := layout.Context{
		Ops: ops,
		Constraints: layout.Constraints{
			Max: image.Pt(0, 0),
		},
	}
	dimsZero := widget.Layout(gtxZero)
	if dimsZero.Size.X != 0 || dimsZero.Size.Y != 0 {
		t.Fatalf("expected 0x0 layout dimensions for zero constraints, got %v", dimsZero.Size)
	}
}
