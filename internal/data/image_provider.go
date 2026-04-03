package data

import (
	"bytes"
	"image"
	_ "image/jpeg"
	_ "image/png"
	"gioui.org/op/paint"
)

// RequestImage pulls an asset from the VFS and decodes it for the GPU.
func RequestImage(path string) (paint.ImageOp, error) {
	pm := GetPakManager()
	data, err := pm.GetFileBytes(path)
	if err != nil {
		return paint.ImageOp{}, err
	}

	img, _, err := image.Decode(bytes.NewReader(data))
	if err != nil {
		return paint.ImageOp{}, err
	}

	return paint.NewImageOp(img), nil
}
