package ui

import (
	"gioui.org/f32"
	"gioui.org/layout"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/net"
)

type WindowManager struct {
	Windows []*widgets.Window
}

var (
	wmInstance *WindowManager
)

func GetWindowManager() *WindowManager {
	if wmInstance == nil {
		wmInstance = &WindowManager{Windows: make([]*widgets.Window, 0)}
	}
	return wmInstance
}

// SpawnWindow adds a window locally and broadcasts to the P2P Mesh.
func (wm *WindowManager) SpawnWindow(id, title string, pos, size f32.Point, isRemote bool) {
	newWin := &widgets.Window{ID: id, Title: title, Pos: pos, Size: size}
	wm.Windows = append(wm.Windows, newWin)

	if !isRemote {
		// --- MULTIPLAYER BROADCAST ---
		payload := map[string]interface{}{
			"type":  "window_spawn",
			"id":    id,
			"title": title,
			"x":     pos.X,
			"y":     pos.Y,
			"w":     size.X,
			"h":     size.Y,
		}
		net.GetMeshNode().Broadcast(payload)
	}
}

func (wm *WindowManager) Layout(gtx layout.Context, th theme.Theme) {
	for _, win := range wm.Windows {
		win.Layout(gtx, th)
	}
}
