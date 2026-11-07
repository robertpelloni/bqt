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
