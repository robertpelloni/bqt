package widgets

import (
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestDemoSurfaceInitialization(t *testing.T) {
	ds := NewDemoSurface()
	if ds == nil {
		t.Fatal("Expected NewDemoSurface to return non-nil instance")
	}

	if ds.synthWidget == nil {
		t.Errorf("Expected synthWidget to be initialized")
	}

	ds.SetSynthVoices(8)
	if ds.synthWidget.activeVoices != 8 {
		t.Errorf("Expected activeVoices to be 8, got %d", ds.synthWidget.activeVoices)
	}

	if ds.VolumeLevel != 0.80 {
		t.Errorf("Expected initial volume 0.80, got %f", ds.VolumeLevel)
	}
	if ds.ClockBpm != 120.0 {
		t.Errorf("Expected initial BPM 120, got %f", ds.ClockBpm)
	}
	if ds.Polyphony != 8 {
		t.Errorf("Expected initial Polyphony 8, got %d", ds.Polyphony)
	}
}

func TestDemoSurfaceLayout(t *testing.T) {
	ds := NewDemoSurface()
	var ops op.Ops
	gtx := layout.Context{Ops: &ops}
	th := theme.GetTheme(theme.Cyberpunk)

	dims := ds.Layout(gtx, th)
	if dims.Size.X < 0 || dims.Size.Y < 0 {
		t.Errorf("Invalid layout dimensions: %v", dims)
	}

	// Verify initial status message
	if ds.StatusMessage == "" {
		t.Errorf("Expected non-empty StatusMessage")
	}
}

func TestDemoSurfaceInteractiveControls(t *testing.T) {
	ds := NewDemoSurface()

	// Direct adjustments
	initialVol := ds.VolumeLevel
	ds.VolumeLevel += 0.05
	if ds.VolumeLevel <= initialVol {
		t.Errorf("Expected VolumeLevel to increase")
	}

	initialBpm := ds.ClockBpm
	ds.ClockBpm += 10.0
	if ds.ClockBpm != initialBpm+10.0 {
		t.Errorf("Expected ClockBpm to increase by 10")
	}

	ds.SetSynthVoices(16)
	if ds.Polyphony != 16 {
		t.Errorf("Expected Polyphony to be 16")
	}
}

func TestDemoSurfaceButtonInteractions(t *testing.T) {
	ds := NewDemoSurface()
	var ops op.Ops
	gtx := layout.Context{Ops: &ops}
	th := theme.GetTheme(theme.Cyberpunk)

	// Test Volume Up/Down clicks
	initVol := ds.VolumeLevel
	ds.btnVolUp.Click()
	ds.Layout(gtx, th)
	if ds.VolumeLevel <= initVol {
		t.Errorf("expected volume to increase on btnVolUp click, got %f", ds.VolumeLevel)
	}

	ds.btnVolDown.Click()
	ds.Layout(gtx, th)
	if ds.VolumeLevel > 0.85 {
		t.Errorf("expected volume to decrease on btnVolDown click, got %f", ds.VolumeLevel)
	}

	// Test BPM Up/Down clicks
	initBpm := ds.ClockBpm
	ds.btnBpmUp.Click()
	ds.Layout(gtx, th)
	if ds.ClockBpm != initBpm+5.0 {
		t.Errorf("expected BPM to increase by 5, got %f", ds.ClockBpm)
	}

	ds.btnBpmDown.Click()
	ds.Layout(gtx, th)
	if ds.ClockBpm != initBpm {
		t.Errorf("expected BPM to return to %f, got %f", initBpm, ds.ClockBpm)
	}

	// Test Polyphony toggle click (cycles 8 -> 16 -> 32 -> 64 -> 4 -> 8)
	ds.btnPolyToggle.Click()
	ds.Layout(gtx, th)
	if ds.Polyphony != 16 {
		t.Errorf("expected Polyphony to cycle to 16, got %d", ds.Polyphony)
	}

	// Test AudioGraph and Kernel button clicks
	ds.btnAudioGraph.Click()
	ds.Layout(gtx, th)
	if ds.StatusMessage == "" {
		t.Errorf("expected status message update on btnAudioGraph click")
	}

	ds.btnTimeMachine.Click()
	ds.Layout(gtx, th)
	if ds.StatusMessage == "" {
		t.Errorf("expected status message update on btnTimeMachine click")
	}

	ds.btnMeshNode.Click()
	ds.Layout(gtx, th)
	if ds.StatusMessage == "" {
		t.Errorf("expected status message update on btnMeshNode click")
	}

	ds.btnDialog.Click()
	ds.Layout(gtx, th)
	if ds.StatusMessage == "" {
		t.Errorf("expected status message update on btnDialog click")
	}
}
