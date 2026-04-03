package widgets

import (
	"image"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"gioui.org/io/pointer"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type Window struct {
	Title    string
	Pos      f32.Point
	Size     f32.Point
	Dragging bool
	DragID   pointer.ID
	Grabbed  bool
}

func (w *Window) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO POINTER INTERCEPTION (Dragging Logic) ---
	for _, ev := range gtx.Events(w) {
		if e, ok := ev.(pointer.Event); ok {
			switch e.Type {
			case pointer.Press:
				w.Dragging = true
				w.DragID = e.PointerID
			case pointer.Release:
				w.Dragging = false
			case pointer.Move:
				if w.Dragging && e.PointerID == w.DragID {
					w.Pos = w.Pos.Add(e.Position)
				}
			}
		}
	}

	// Register pointer hit area for the title bar
	titleBarRect := f32.Rect(w.Pos.X, w.Pos.Y, w.Pos.X+w.Size.X, w.Pos.Y+30)
	defer pointer.Rect(image.Rectangle{Min: titleBarRect.Min.Round(), Max: titleBarRect.Max.Round()}).Push(gtx.Ops).Pop()
	pointer.InputOp{Tag: w, Types: pointer.Press | pointer.Release | pointer.Move}.Add(gtx.Ops)

	// --- THEMATIC GEOMETRY PASS (Go Port) ---
	var path clip.Path
	path.Begin(gtx.Ops)
	cut := float32(20.0)
	
	// Position the context
	defer op.Offset(image.Pt(int(w.Pos.X), int(w.Pos.Y))).Push(gtx.Ops).Pop()

	path.MoveTo(f32.Pt(cut, 0))
	path.LineTo(f32.Pt(w.Size.X, 0))
	path.LineTo(f32.Pt(w.Size.X, w.Size.Y-cut))
	path.LineTo(f32.Pt(w.Size.X-cut, w.Size.Y))
	path.LineTo(f32.Pt(0, w.Size.Y))
	path.LineTo(f32.Pt(0, cut))
	path.Close()

	// Fill and Stroke with High-Art Theming
	stack := clip.Outline{Path: path.End()}.Op().Push(gtx.Ops)
	paint.Fill(gtx.Ops, th.Surface)
	stack.Pop()

	paint.ColorOp{Color: th.Primary}.Add(gtx.Ops)
	clip.Stroke{Path: path.End(), Width: 1.5}.Op().Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)

	return layout.Dimensions{Size: image.Pt(int(w.Size.X), int(w.Size.Y))}
}
