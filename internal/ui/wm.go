package ui

import (
	"image"

	"gioui.org/f32"
	"gioui.org/layout"
	"gioui.org/op"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
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
			Tabs:    &widgets.TabBar{Tabs: []string{"Kernel", "Terminal", "Docs"}},
		}
	}
	return wmInstance
}

func (wm *WindowManager) Reset() {
	wm.Windows = make([]*widgets.Window, 0)
	wm.Tabs = &widgets.TabBar{Tabs: []string{"Kernel", "Terminal", "Docs"}}
}

func (wm *WindowManager) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if wm == nil {
		return layout.Dimensions{}
	}

	if wm.Tabs != nil {
		stack := op.Offset(image.Pt(16, 16)).Push(gtx.Ops)
		wm.Tabs.Layout(gtx, th)
		stack.Pop()
	}

	for _, win := range wm.Windows {
		win.Layout(gtx, th)
	}
	return layout.Dimensions{Size: gtx.Constraints.Max}
}

func (wm *WindowManager) SpawnWindow(id, title string, pos, size f32.Point) {
	wm.Windows = append(wm.Windows, &widgets.Window{ID: id, Title: title, Pos: pos, Size: size})
}
