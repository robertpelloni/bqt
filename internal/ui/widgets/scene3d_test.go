package widgets

import (
	"image"
	"testing"

	"gioui.org/f32"
	"gioui.org/layout"
	"gioui.org/op"
)

type mock3DNode struct {
	rendered bool
}

func (m *mock3DNode) Render(gtx layout.Context) {
	m.rendered = true
}

func TestScene3D_Layout(t *testing.T) {
	mockNode := &mock3DNode{}
	mesh := &MeshNode{
		Vertices: []f32.Point{
			{X: 0, Y: 0},
			{X: 1, Y: 0},
			{X: 0, Y: 1},
		},
		Pos: f32.Point{X: 10, Y: 20},
	}

	scene := &Scene3D{
		Nodes: []Node3D{
			mockNode,
			mesh,
		},
		Camera: f32.Point{X: 0, Y: 5},
	}

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops:         ops,
		Constraints: layout.Exact(image.Point{X: 1024, Y: 768}),
	}

	dims := scene.Layout(gtx, nil)
	if dims.Size.X != 1024 || dims.Size.Y != 768 {
		t.Errorf("expected dims 1024x768, got %v", dims.Size)
	}

	if !mockNode.rendered {
		t.Errorf("expected mock 3D node Render to be executed")
	}

	// Directly invoke MeshNode Render to verify safe execution
	mesh.Render(gtx)
}
