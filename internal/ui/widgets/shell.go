package widgets

import (
	"image"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type Shell struct {
	StartMenuVisible bool
}

func (s *Shell) LayoutTaskbar(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO TASKBAR RENDER PASS ---
	height := gtx.Dp(unit.Dp(48))
	rect := image.Rect(0, gtx.Constraints.Max.Y-height, gtx.Constraints.Max.X, gtx.Constraints.Max.Y)
	
	// Background
	defer clip.Rect(rect).Push(gtx.Ops).Pop()
	paint.ColorOp{Color: th.Surface}.Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)
	
	// Top Border Accent
	border := image.Rect(0, gtx.Constraints.Max.Y-height, gtx.Constraints.Max.X, gtx.Constraints.Max.Y-height+2)
	defer clip.Rect(border).Push(gtx.Ops).Pop()
	paint.ColorOp{Color: th.Primary}.Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)

	return layout.Dimensions{Size: rect.Size()}
}

func (s *Shell) LayoutStartMenu(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if !s.StartMenuVisible {
		return layout.Dimensions{}
	}

	// --- GO START MENU RENDER PASS ---
	width := gtx.Dp(unit.Dp(250))
	height := gtx.Dp(unit.Dp(400))
	rect := image.Rect(10, gtx.Constraints.Max.Y-gtx.Dp(unit.Dp(58))-height, 10+width, gtx.Constraints.Max.Y-gtx.Dp(unit.Dp(58)))

	defer clip.Rect(rect).Push(gtx.Ops).Pop()
	paint.ColorOp{Color: th.Surface}.Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)

	// Angled Border
	var path clip.Path
	path.Begin(gtx.Ops)
	// ... (simplified geometry for shell parity)
	
	return layout.Dimensions{Size: rect.Size()}
}
