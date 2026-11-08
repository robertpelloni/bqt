package widgets

import (
	"image"
	"testing"

	"gioui.org/f32"
	"gioui.org/layout"
	"gioui.org/op"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestNodeEditor_Layout_Empty(t *testing.T) {
	editor := &NodeEditor{
		Nodes: []Node{},
	}

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops:         ops,
		Constraints: layout.Exact(image.Point{X: 640, Y: 480}),
	}
	th := theme.GetTheme(theme.Dark)

	dims := editor.Layout(gtx, th)
	if dims.Size.X != 640 || dims.Size.Y != 480 {
		t.Errorf("expected dims 640x480, got %v", dims.Size)
	}
}

func TestNodeEditor_Layout_WithNodes(t *testing.T) {
	editor := &NodeEditor{
		Nodes: []Node{
			{
				ID:    "node-1",
				Pos:   f32.Point{X: 10, Y: 20},
				Label: "Oscillator 1",
			},
			{
				ID:    "node-2",
				Pos:   f32.Point{X: 150, Y: 200},
				Label: "Filter 1",
			},
		},
	}

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops:         ops,
		Constraints: layout.Exact(image.Point{X: 800, Y: 600}),
	}
	th := theme.GetTheme(theme.Dark)

	dims := editor.Layout(gtx, th)
	if dims.Size.X != 800 || dims.Size.Y != 600 {
		t.Errorf("expected dims 800x600, got %v", dims.Size)
	}

	if len(editor.Nodes) != 2 {
		t.Errorf("expected 2 nodes, got %d", len(editor.Nodes))
	}
}
