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
	login  *widgets.LoginView
}

func NewEngine() *Engine {
	return &Engine{
		window: app.NewWindow(app.Title("OmniUI Go - Security & Time")),
		login:  &widgets.LoginView{},
	}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)
	im := kernel.GetInputManager()
	
	// Desktop Widgets
	cal := &widgets.Calendar{}

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			paint.Fill(gtx.Ops, th.Background)

			if !e.login.Authenticated {
				// 1. Render OS Auth Guard
				e.login.Layout(gtx, th)
			} else {
				// 2. Render Secure Desktop Area
				cal.Layout(gtx, th)
				// ... (rest of desktop logic)
			}

			// 3. Render Multi-Cursor
			for _, dev := range im.GetDevices() {
				// ... (rendering logic)
			}

			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
