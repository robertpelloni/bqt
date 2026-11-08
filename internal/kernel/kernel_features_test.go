package kernel

import (
	"testing"
)

func TestKernelFeatures_ActionRegistry(t *testing.T) {
	ar := GetActionRegistry()
	if ar == nil {
		t.Fatalf("expected GetActionRegistry to return non-nil instance")
	}

	triggered := false
	var triggeredUser string
	ar.RegisterAction("cmd.save", "Save Project", "Ctrl+S", func(userID string) {
		triggered = true
		triggeredUser = userID
	})

	ar.Trigger("cmd.save", "user_alpha")
	if !triggered {
		t.Errorf("expected action cmd.save to be triggered")
	}
	if triggeredUser != "user_alpha" {
		t.Errorf("expected user_alpha, got %q", triggeredUser)
	}

	// Trigger non-existent action safely
	ar.Trigger("cmd.nonexistent", "user_beta")
}

func TestKernelFeatures_AccessibilityManager(t *testing.T) {
	am := GetAccessibilityManager()
	if am == nil {
		t.Fatalf("expected GetAccessibilityManager to return non-nil instance")
	}

	domPayload := `{"nodes":[{"id":"btn_ok","role":"button","label":"OK"}]}`
	am.UpdateTree(domPayload)
}

func TestKernelFeatures_SearchEngine(t *testing.T) {
	se := GetSearchEngine()
	if se == nil {
		t.Fatalf("expected GetSearchEngine to return non-nil instance")
	}

	results := se.GlobalSearch("Omni")
	if len(results) == 0 {
		t.Errorf("expected non-empty search results from GlobalSearch")
	}
	if results[0].Source != "Local" {
		t.Errorf("expected Local source hit, got %q", results[0].Source)
	}
}

func TestKernelFeatures_PropertyReactiveBindings(t *testing.T) {
	p1 := NewProperty[int](10)
	if p1.Get() != 10 {
		t.Fatalf("expected initial value 10, got %d", p1.Get())
	}

	listenerVal := 0
	p1.AddListener(func(v int) {
		listenerVal = v
	})

	p1.Set(25)
	if p1.Get() != 25 {
		t.Errorf("expected 25, got %d", p1.Get())
	}
	if listenerVal != 25 {
		t.Errorf("expected listener to be notified with 25, got %d", listenerVal)
	}

	// Test binding p2 to p1
	p2 := NewProperty[int](0)
	p2.Bind(p1)
	if p2.Get() != 25 {
		t.Errorf("expected bound p2 to take initial value 25 from p1, got %d", p2.Get())
	}

	p1.Set(50)
	if p2.Get() != 50 {
		t.Errorf("expected bound p2 to update to 50 when p1 updates, got %d", p2.Get())
	}
}
