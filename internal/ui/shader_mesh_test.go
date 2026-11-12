package ui

import (
	"testing"
)

func TestShaderSync_BroadcastAndRemote(t *testing.T) {
	sync := GetShaderSync()
	if sync == nil {
		t.Fatalf("expected GetShaderSync to return non-nil instance")
	}

	shaderSrc := "void main() { gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); }"
	sync.BroadcastShaderUpdate("panel_1", shaderSrc)

	if sync.ActiveShaders["panel_1"] != shaderSrc {
		t.Errorf("expected active shader to be updated, got %s", sync.ActiveShaders["panel_1"])
	}

	remoteSrc := "void main() { gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); }"
	sync.HandleRemoteShader("peer_99", "panel_2", remoteSrc)

	if sync.ActiveShaders["panel_2"] != remoteSrc {
		t.Errorf("expected remote shader to be updated, got %s", sync.ActiveShaders["panel_2"])
	}
}
