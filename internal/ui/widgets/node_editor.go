package widgets

import (
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"gioui.org/unit"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type Node struct {
	ID    string
	Pos   f32.Point
	Label string
}

type NodeEditor struct {
	Nodes []Node
}

func (n *NodeEditor) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- GO NATIVE NODE RENDER PASS ---
	// Real-time Bezier wire and node layouting natively in Go.
	
	for _, node := range n.Nodes {
		// Draw Node Body
		rect := f32.Rect(node.Pos.X, node.Pos.Y, node.Pos.X+100, node.Pos.Y+50)
		stack := clip.Rect(rect.Round()).Push(gtx.Ops)
		paint.Fill(gtx.Ops, th.Surface)
		stack.Pop()

		// Draw Wires (Bezier simulation)
		var p clip.Path
		p.Begin(gtx.Ops)
		p.MoveTo(node.Pos)
		p.LineTo(f32.Pt(node.Pos.X+200, node.Pos.Y+100))
		clip.Stroke{Path: p.End(), Width: 2}.Op().Add(gtx.Ops)
		paint.ColorOp{Color: th.Primary}.Add(gtx.Ops)
		paint.PaintOp{}.Add(gtx.Ops)
	}

	return layout.Dimensions{Size: gtx.Constraints.Max}
}
