package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestDialogVisibilityAndLayout(t *testing.T) {
	d := &Dialog{
		Visible: false,
		Title:   "Warning",
		Body:    "System reboot required.",
	}

	th := theme.GetTheme(theme.Dark)
	var ops op.Ops
	gtx := layout.Context{
		Ops:         &ops,
		Constraints: layout.Exact(image.Pt(640, 480)),
	}

	// 1. Invisible dialog returns zero dimensions and empty result
	dims, res := d.Layout(gtx, th)
	if dims.Size.X != 0 || dims.Size.Y != 0 || res != "" {
		t.Errorf("Expected zero dimensions and empty result when not visible, got %v, %s", dims, res)
	}

	// 2. Visible dialog returns centered dimensions
	d.Visible = true
	dims, res = d.Layout(gtx, th)
	if dims.Size.X == 0 || dims.Size.Y == 0 {
		t.Errorf("Expected non-zero dimensions when visible, got %v", dims)
	}
	if res != "" {
		t.Errorf("Expected empty result when no button clicked, got %s", res)
	}
	if !d.Visible {
		t.Errorf("Expected dialog to remain visible until button click")
	}
}

func TestDialogButtonInteractions(t *testing.T) {
	d := &Dialog{
		Visible: true,
		Title:   "Confirm Action",
		Body:    "Apply changes to system configuration?",
	}

	th := theme.GetTheme(theme.Dark)
	var ops op.Ops
	gtx := layout.Context{
		Ops:         &ops,
		Constraints: layout.Exact(image.Pt(640, 480)),
	}

	// 1. Simulate Accept click
	d.Accept.Click()
	dims, res := d.Layout(gtx, th)
	if res != "accept" {
		t.Fatalf("expected result 'accept', got %q", res)
	}
	if d.Visible {
		t.Fatal("expected dialog to close (Visible=false) after Accept click")
	}
	if dims.Size.X == 0 || dims.Size.Y == 0 {
		t.Errorf("expected non-zero layout dimensions on dismiss frame")
	}

	// 2. Simulate Cancel click
	d.Visible = true
	d.Cancel.Click()
	_, res = d.Layout(gtx, th)
	if res != "cancel" {
		t.Fatalf("expected result 'cancel', got %q", res)
	}
	if d.Visible {
		t.Fatal("expected dialog to close (Visible=false) after Cancel click")
	}
}
