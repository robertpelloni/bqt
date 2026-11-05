package net

import (
	"image"
	"image/color"
	"testing"
)

func TestStreamAssetToMesh(t *testing.T) {
	// Create a small 2x2 synthetic RGBA test image
	img := image.NewRGBA(image.Rect(0, 0, 2, 2))
	img.Set(0, 0, color.RGBA{R: 255, A: 255})
	img.Set(1, 1, color.RGBA{B: 255, A: 255})

	// Stream asset to mesh (with zero peers, should complete safely without panic)
	StreamAssetToMesh("test_icon.png", img)

	// Simulate receiving a remote asset
	dummyPNGBytes := []byte{0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}
	HandleRemoteAsset("peer-123", "test_icon.png", dummyPNGBytes)
}
