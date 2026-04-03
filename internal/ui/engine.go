package ui

import (
	"gioui.org/app"
	"gioui.org/io/system"
	"gioui.org/io/key"
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
	return &Engine{
		window: app.NewWindow(app.Title("OmniUI Go - Multi-User OS")),
	}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)
	im := kernel.GetInputManager()
	undo := kernel.GetUndoStack()
	
	doc := &widgets.MarkdownView{Text: "# Welcome to BobUI Go\n## Distributed Desktop Active"}
	testWin := &widgets.Window{Title: "Documentation", Pos: f32.Pt(50, 50), Size: f32.Pt(600, 400)}

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case key.Event:
			// --- MULTI-USER UNDO SHORTCUT ---
			if tag.Name == "Z" && tag.Modifiers.Contains(key.ModShortcut) && tag.State == key.Press {
				undo.Undo("sys-admin")
			}
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			paint.Fill(gtx.Ops, th.Background)

			// Render Doc Window
			testWin.Layout(gtx, th)
			doc.Layout(gtx, th)

			// Multi-Cursor Overlay
			for _, dev := range im.GetDevices() {
				// ... (cursor rendering)
			}

			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
