package ui

import (
	"image"
	"testing"

	"gioui.org/f32"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

func TestWindowManagerResetSpawnAndLayout(t *testing.T) {
	wm := &WindowManager{}
	wm.Reset()
	if len(wm.Windows) != 0 {
		t.Fatalf("expected reset window manager to start with no windows, got %d", len(wm.Windows))
	}
	if wm.Tabs == nil || len(wm.Tabs.Tabs) == 0 {
		t.Fatal("expected reset window manager to initialize tabs")
	}

	wm.SpawnWindow("win-1", "Inspector", f32.Pt(10, 20), f32.Pt(200, 120))
	wm.SpawnWindow("win-2", "Docs", f32.Pt(30, 40), f32.Pt(240, 140))
	if len(wm.Windows) != 2 {
		t.Fatalf("expected 2 spawned windows, got %d", len(wm.Windows))
	}
	if wm.Windows[0].Title != "Inspector" || wm.Windows[1].Title != "Docs" {
		t.Fatalf("unexpected window titles: %+v %+v", wm.Windows[0], wm.Windows[1])
	}

	gtx := newTestLayoutContext(image.Pt(1280, 900))
	dims := wm.Layout(gtx, theme.GetTheme(theme.Cyberpunk))
	if dims.Size.X != 1280 || dims.Size.Y != 900 {
		t.Fatalf("expected window manager layout to cover full available space, got %v", dims.Size)
	}
}
