package ui

import (
	"gioui.org/app"
	"gioui.org/io/system"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"github.com/robertpelloni/bobui/internal/kernel"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
)

type Engine struct {
	window *app.Window
}

func NewEngine() *Engine {
	return &Engine{window: app.NewWindow(app.Title("OmniUI Go - Kernel Governance"))}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)
	
	// Administrative Widgets
	admin := &widgets.AdminShell{UserList: []string{"Local Admin", "Remote Peer 1"}}
	rhi := &widgets.RHINode{}
	
	testWin := &widgets.Window{ID: "win_admin", Title: "Permission Manager", Pos: f32.Pt(50, 50), Size: f32.Pt(400, 300)}

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			paint.Fill(gtx.Ops, th.Background)

			// 1. Render Admin Governance Window
			testWin.Layout(gtx, th)
			admin.Layout(gtx, th)

			// 2. Render RHI Hardware Node (Visualization)
			rhi.Layout(gtx)

			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
