package widgets

import (
	"image"
	"image/color"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/f32"
)

// DrawGrabGlow renders a multi-pass neon bloom around a widget.
func DrawGrabGlow(gtx layout.Context, rect f32.Rectangle, accent color.NRGBA) {
	// --- GO BLOOM RENDER PASS ---
	// We draw concentric layers with decreasing opacity to simulate light bleed.
	for i := 1; i <= 5; i++ {
		offset := float32(i) * 1.5
		glowRect := f32.Rect(rect.Min.X-offset, rect.Min.Y-offset, rect.Max.X+offset, rect.Max.Y+offset)
		
		// Map index to decreasing alpha
		alpha := uint8(100 / (i * 2))
		glowColor := accent
		glowColor.A = alpha

		stack := clip.Rect(glowRect.Round()).Push(gtx.Ops)
		paint.Fill(gtx.Ops, glowColor)
		stack.Pop()
	}
}
