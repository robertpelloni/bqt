package ui

import (
	"gioui.org/f32"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"gioui.org/layout"
)

type WindowManager struct {
	Windows []*widgets.Window
}

var (
	wmInstance *WindowManager
)

func GetWindowManager() *WindowManager {
	if wmInstance == nil {
		wmInstance = &WindowManager{
			Windows: make([]*widgets.Window, 0),
		}
	}
	return wmInstance
}

// SpawnWindow injects a new application window into the Go desktop.
func (wm *WindowManager) SpawnWindow(title string, pos f32.Point, size f32.Point) {
	newWin := &widgets.Window{
		ID:    title,
		Title: title,
		Pos:   pos,
		Size:  size,
	}
	wm.Windows = append(wm.Windows, newWin)
}

func (wm *WindowManager) Layout(gtx layout.Context, th theme.Theme) {
	for _, win := range wm.Windows {
		win.Layout(gtx, th)
	}
}
