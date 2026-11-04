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
