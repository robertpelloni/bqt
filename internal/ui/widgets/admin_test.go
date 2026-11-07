package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestAdminShellLayout(t *testing.T) {
	adminEmpty := &AdminShell{}

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 400,
			Y: 300,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Dark)

	// Layout with exact constraints satisfies list constraints
	dimsEmpty := adminEmpty.Layout(gtx, th)
	if dimsEmpty.Size.X != 400 || dimsEmpty.Size.Y != 300 {
		t.Fatalf("expected exact constraints (400, 300), got %+v", dimsEmpty.Size)
	}

	// Populated user list
	admin := &AdminShell{
		UserList: []string{"root", "peer-node-1", "guest"},
	}

	dims := admin.Layout(gtx, th)
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions, got %+v", dims.Size)
	}
}
