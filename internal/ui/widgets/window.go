package widgets

import (
	"image"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"gioui.org/unit"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type Window struct {
	Title      string
	Pos        f32.Point
	Size       f32.Point
	Dragging   bool
	Grabbed    bool // UI feedback state
	OwnerColor theme.ThemeType
}

func (w *Window) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO GRAB GLOW PASS ---
	if w.Grabbed {
		// Draw a semi-transparent pulsing halo
		var glowPath clip.Path
		glowPath.Begin(gtx.Ops)
		offset := float32(4.0)
		glowPath.MoveTo(f32.Pt(w.Pos.X-offset, w.Pos.Y-offset))
		glowPath.LineTo(f32.Pt(w.Pos.X+w.Size.X+offset, w.Pos.Y-offset))
		glowPath.LineTo(f32.Pt(w.Pos.X+w.Size.X+offset, w.Pos.Y+w.Size.Y+offset))
		glowPath.LineTo(f32.Pt(w.Pos.X-offset, w.Pos.Y+w.Size.Y+offset))
		glowPath.Close()
		
		stack := clip.Outline{Path: glowPath.End()}.Op().Push(gtx.Ops)
		paint.ColorOp{Color: th.Accent}.Add(gtx.Ops) // Pulsing user color
		paint.PaintOp{}.Add(gtx.Ops)
		stack.Pop()
	}

	// Standard Window Geometry
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

	// Fill and Stroke
	stack := clip.Outline{Path: path.End()}.Op().Push(gtx.Ops)
	paint.ColorOp{Color: th.Surface}.Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)
	stack.Pop()

	paint.ColorOp{Color: th.Primary}.Add(gtx.Ops)
	clip.Stroke{Path: path.End(), Width: 1.5}.Op().Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)

	return layout.Dimensions{Size: image.Pt(int(w.Size.X), int(w.Size.Y))}
}
