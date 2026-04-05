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

	wm.SpawnManagedWindow("win-1", "Inspector", "Kernel", "kernel-body", f32.Pt(10, 20), f32.Pt(200, 120))
	wm.SpawnManagedWindow("win-2", "Docs", "Docs", "docs-body", f32.Pt(30, 40), f32.Pt(240, 140))
	wm.SpawnWindow("win-3", "Shared", f32.Pt(50, 60), f32.Pt(180, 100))
	if len(wm.Windows) != 3 {
		t.Fatalf("expected 3 spawned windows, got %d", len(wm.Windows))
	}
	if wm.Windows[0].Title != "Inspector" || wm.Windows[1].Title != "Docs" || wm.Windows[2].Title != "Shared" {
		t.Fatalf("unexpected window titles: %+v %+v %+v", wm.Windows[0], wm.Windows[1], wm.Windows[2])
	}

	if active := wm.ActiveTab(); active != "Kernel" {
		t.Fatalf("expected initial active tab Kernel, got %q", active)
	}
	visible := wm.VisibleWindows()
	if len(visible) != 2 {
		t.Fatalf("expected Kernel-tab + shared window to be visible, got %d", len(visible))
	}
	if visible[0].ID != "win-1" || visible[1].ID != "win-3" {
		t.Fatalf("unexpected visible windows for Kernel tab: %+v %+v", visible[0], visible[1])
	}

	wm.Tabs.ClickTab(2)
	gtx := newTestLayoutContext(image.Pt(1280, 900))
	wm.Layout(gtx, theme.GetTheme(theme.Cyberpunk))
	if active := wm.ActiveTab(); active != "Docs" {
		t.Fatalf("expected active tab Docs after click, got %q", active)
	}
	visible = wm.VisibleWindows()
	if len(visible) != 2 {
		t.Fatalf("expected Docs-tab + shared window to be visible, got %d", len(visible))
	}
	if visible[0].ID != "win-2" || visible[1].ID != "win-3" {
		t.Fatalf("unexpected visible windows for Docs tab: %+v %+v", visible[0], visible[1])
	}

	dims := wm.Layout(newTestLayoutContext(image.Pt(1280, 900)), theme.GetTheme(theme.Cyberpunk))
	if dims.Size.X != 1280 || dims.Size.Y != 900 {
		t.Fatalf("expected window manager layout to cover full available space, got %v", dims.Size)
	}
}
