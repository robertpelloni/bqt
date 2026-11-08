package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestDockArea_Layout(t *testing.T) {
	dock := &DockArea{
		Name: "MainDock",
	}

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops:         ops,
		Constraints: layout.Exact(image.Point{X: 800, Y: 600}),
	}
	th := theme.GetTheme(theme.Dark)

	childCalled := false
	expectedDims := layout.Dimensions{Size: image.Point{X: 500, Y: 400}}

	childWidget := func(gtx layout.Context) layout.Dimensions {
		childCalled = true
		return expectedDims
	}

	dims := dock.Layout(gtx, th, childWidget)

	if !childCalled {
		t.Errorf("expected dock child widget to be rendered, but it was not called")
	}

	if dims != expectedDims {
		t.Errorf("expected dims %v, got %v", expectedDims, dims)
	}

	if dock.Name != "MainDock" {
		t.Errorf("expected dock name 'MainDock', got '%s'", dock.Name)
	}
}
