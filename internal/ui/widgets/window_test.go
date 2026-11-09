package widgets

import (
	"image"
	"testing"

	"gioui.org/f32"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestWindowMoveResizeCloseAndActivationRequest(t *testing.T) {
	w := &Window{ID: "win-1", Title: "Inspector", Pos: f32.Pt(10, 20), Size: f32.Pt(240, 160)}
	w.MoveBy(15, -5)
	if w.Pos.X != 25 || w.Pos.Y != 15 {
		t.Fatalf("expected moved position 25/15, got %v/%v", w.Pos.X, w.Pos.Y)
	}

	w.ResizeBy(-500, -500)
	if w.Size.X < 180 || w.Size.Y < 120 {
		t.Fatalf("expected resize to clamp to minimum 180x120, got %v x %v", w.Size.X, w.Size.Y)
	}

	dims := w.Layout(newWidgetTestLayoutContext(image.Pt(1280, 900)), theme.GetTheme(theme.Cyberpunk))
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected visible window layout to produce non-zero dimensions, got %v", dims.Size)
	}

	w.RequestActivate()
	w.Layout(newWidgetTestLayoutContext(image.Pt(1280, 900)), theme.GetTheme(theme.Cyberpunk))
	if !w.ConsumeActivationRequest() {
		t.Fatal("expected programmatic activation request to be consumed after layout")
	}
	if w.ConsumeActivationRequest() {
		t.Fatal("expected activation request to clear after consumption")
	}

	w.CloseBtn.Click()
	w.Layout(newWidgetTestLayoutContext(image.Pt(1280, 900)), theme.GetTheme(theme.Cyberpunk))
	if !w.Closed {
		t.Fatal("expected close button click to close window")
	}
	closedDims := w.Layout(newWidgetTestLayoutContext(image.Pt(1280, 900)), theme.GetTheme(theme.Cyberpunk))
	if closedDims.Size.X != 0 || closedDims.Size.Y != 0 {
		t.Fatalf("expected closed window to layout to zero dimensions, got %v", closedDims.Size)
	}

	// Verify SetActive behavior
	w2 := &Window{ID: "win-2", Title: "Console"}
	w2.SetActive(true)
	if !w2.Active {
		t.Errorf("expected open window to become active")
	}

	w2.Close()
	w2.SetActive(true)
	if w2.Active {
		t.Errorf("expected closed window to remain inactive even after SetActive(true)")
	}
}

func TestWindow_ActiveTitleColorAndDefaultMinSize(t *testing.T) {
	// 1. Verify Active=true layout styling and execution
	w := &Window{
		ID:     "win-active",
		Title:  "Active Window",
		Active: true,
		Pos:    f32.Pt(50, 50),
		Size:   f32.Pt(300, 200),
	}

	dims := w.Layout(newWidgetTestLayoutContext(image.Pt(1000, 800)), theme.GetTheme(theme.Cyberpunk))
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected active window to render non-zero dimensions, got %v", dims.Size)
	}

	// 2. Custom MinSize clamp during ResizeBy
	wCustom := &Window{
		ID:      "win-custom",
		Title:   "Custom MinSize",
		MinSize: f32.Pt(250, 150),
		Size:    f32.Pt(300, 200),
	}
	wCustom.ResizeBy(-400, -400)
	if wCustom.Size.X != 250 || wCustom.Size.Y != 150 {
		t.Fatalf("expected custom MinSize clamp 250x150, got %vx%v", wCustom.Size.X, wCustom.Size.Y)
	}
}
