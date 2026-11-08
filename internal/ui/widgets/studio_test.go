package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestOmniStudio_NewAndLayout(t *testing.T) {
	studio := NewOmniStudio()
	if studio == nil {
		t.Fatalf("expected NewOmniStudio to return non-nil instance")
	}

	if studio.Editor == nil || studio.Terminal == nil || studio.Explorer == nil || studio.SplitView == nil {
		t.Errorf("expected all nested components to be initialized, got %+v", studio)
	}

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops:         ops,
		Constraints: layout.Exact(image.Point{X: 1024, Y: 768}),
	}
	th := theme.GetTheme(theme.Dark)

	dims := studio.Layout(gtx, th)
	if dims.Size.X != 1024 || dims.Size.Y != 768 {
		t.Errorf("expected dims 1024x768, got %v", dims.Size)
	}
}

func TestOmniStudio_NilFallbacks(t *testing.T) {
	// Empty studio with nil child pointers
	studio := &OmniStudio{}

	ops := new(op.Ops)
	gtx := layout.Context{
		Ops:         ops,
		Constraints: layout.Exact(image.Point{X: 800, Y: 600}),
	}
	th := theme.GetTheme(theme.Cyberpunk)

	dims := studio.Layout(gtx, th)
	if dims.Size.X != 800 || dims.Size.Y != 600 {
		t.Errorf("expected dims 800x600, got %v", dims.Size)
	}

	if studio.Editor == nil || studio.Terminal == nil || studio.Explorer == nil || studio.SplitView == nil {
		t.Errorf("expected nil children to be safely initialized on Layout, got %+v", studio)
	}
}
