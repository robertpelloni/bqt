package ui

import (
	"gioui.org/f32"
	"gioui.org/layout"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type WindowManager struct {
	Windows []*widgets.Window
	Tabs    *widgets.TabBar
}

var (
	wmInstance *WindowManager
)

func GetWindowManager() *WindowManager {
	if wmInstance == nil {
		wmInstance = &WindowManager{
			Windows: make([]*widgets.Window, 0),
			Tabs: &widgets.TabBar{Tabs: []string{"Kernel", "Terminal", "Docs"}},
		}
	}
	return wmInstance
}

func (wm *WindowManager) Layout(gtx layout.Context, th theme.Theme) {
	// --- GO DOCKING SYNTHESIS ---
	// Render the global TabBar for docked windows
	wm.Tabs.Layout(gtx, th)

	for _, win := range wm.Windows {
		win.Layout(gtx, th)
	}
}

func (wm *WindowManager) SpawnWindow(id, title string, pos, size f32.Point) {
	wm.Windows = append(wm.Windows, &widgets.Window{ID: id, Title: title, Pos: pos, Size: size})
}
