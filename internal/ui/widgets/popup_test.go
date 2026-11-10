package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestPopupVisibilityAndLayout(t *testing.T) {
	p := &Popup{
		Visible: false,
		Title:   "Notification",
		Body:    "Operation finished.",
	}

	th := theme.GetTheme(theme.Cyberpunk)
	var ops op.Ops
	gtx := layout.Context{
		Ops:         &ops,
		Constraints: layout.Exact(image.Pt(640, 480)),
	}

	// 1. Invisible popup returns zero dimensions
	dims := p.Layout(gtx, th)
	if dims.Size.X != 0 || dims.Size.Y != 0 {
		t.Errorf("Expected zero dimensions when popup is not visible, got %v", dims)
	}

	// 2. Visible popup returns valid centered dimensions
	p.Visible = true
	dims = p.Layout(gtx, th)
	if dims.Size.X == 0 || dims.Size.Y == 0 {
		t.Errorf("Expected non-zero dimensions when popup is visible, got %v", dims)
	}
}

func TestPopupDifferentThemesAndEmptyStrings(t *testing.T) {
	p := &Popup{
		Visible: true,
		Title:   "",
		Body:    "",
	}

	th := theme.GetTheme(theme.Dark)
	var ops op.Ops
	gtx := layout.Context{
		Ops:         &ops,
		Constraints: layout.Exact(image.Pt(500, 300)),
	}

	dims := p.Layout(gtx, th)
	if dims.Size.X == 0 || dims.Size.Y == 0 {
		t.Errorf("expected non-zero dimensions for empty string popup, got %v", dims)
	}
}
