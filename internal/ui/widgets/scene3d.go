package widgets

import (
	"gioui.org/layout"
	"gioui.org/f32"
)

type Node3D interface {
	Render(gtx layout.Context)
}

type MeshNode struct {
	Vertices []f32.Point
	Pos      f32.Point
}

type Scene3D struct {
	Nodes  []Node3D
	Camera f32.Point
}

func (s *Scene3D) Layout(gtx layout.Context, th interface{}) layout.Dimensions {
	for _, node := range s.Nodes {
		node.Render(gtx)
	}
	return layout.Dimensions{Size: gtx.Constraints.Max}
}

func (m *MeshNode) Render(gtx layout.Context) {}
