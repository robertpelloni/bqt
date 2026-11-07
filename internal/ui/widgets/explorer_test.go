package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestMeshExplorerLayout(t *testing.T) {
	explorerEmpty := &MeshExplorer{}

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

	// Layout with empty file entries satisfies exact constraints
	dimsEmpty := explorerEmpty.Layout(gtx, th)
	if dimsEmpty.Size.X != 400 || dimsEmpty.Size.Y != 300 {
		t.Fatalf("expected exact constraints (400, 300), got %+v", dimsEmpty.Size)
	}

	// Populated with local and remote files
	explorer := &MeshExplorer{
		Files: []FileEntry{
			{Name: "kernel.bin", Path: "/boot/kernel.bin", PeerID: ""},
			{Name: "remote_patch.diff", Path: "/shared/patch.diff", PeerID: "peer-99"},
		},
	}

	dims := explorer.Layout(gtx, th)
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions, got %+v", dims.Size)
	}
}
