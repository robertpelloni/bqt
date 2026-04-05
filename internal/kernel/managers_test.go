package kernel

import "testing"

func TestInputManagerRegisterUpdateAndSnapshot(t *testing.T) {
	im := &InputManager{devices: make(map[string]*DeviceState)}
	im.RegisterDevice("mouse-1", "Primary Mouse", Mouse)
	im.RegisterDevice("mouse-1", "Duplicate Mouse", Touch)
	im.RegisterDevice("touch-1", "Touch Surface", Touch)
	im.UpdateCursor("mouse-1", 12.5, 22.5)
	im.UpdateCursor("missing", 99, 101)

	devices := im.GetDevices()
	if len(devices) != 2 {
		t.Fatalf("expected 2 registered devices, got %d", len(devices))
	}

	var foundMouse bool
	for _, dev := range devices {
		if dev.ID == "mouse-1" {
			foundMouse = true
			if dev.Name != "Primary Mouse" {
				t.Fatalf("expected first registration to win, got %q", dev.Name)
			}
			if dev.Type != Mouse {
				t.Fatalf("expected mouse type to remain Mouse, got %q", dev.Type)
			}
			if dev.X != 12.5 || dev.Y != 22.5 {
				t.Fatalf("expected updated cursor position 12.5/22.5, got %v/%v", dev.X, dev.Y)
			}
		}
	}
	if !foundMouse {
		t.Fatal("expected to find mouse-1 in device snapshot")
	}
}

func TestPermissionManagerLockUnlockAndInteraction(t *testing.T) {
	pm := &PermissionManager{}

	if !pm.TryLock("device-a", "target-1") {
		t.Fatal("expected first lock acquisition to succeed")
	}
	if !pm.CanInteract("device-a", "target-1") {
		t.Fatal("expected lock owner to be able to interact")
	}
	if pm.TryLock("device-b", "target-1") {
		t.Fatal("expected second device lock acquisition to fail")
	}
	if pm.CanInteract("device-b", "target-1") {
		t.Fatal("expected non-owner to be blocked from interaction")
	}

	pm.Unlock("device-b", "target-1")
	if !pm.CanInteract("device-a", "target-1") {
		t.Fatal("expected wrong-owner unlock to leave lock intact")
	}

	pm.Unlock("device-a", "target-1")
	if !pm.CanInteract("device-b", "target-1") {
		t.Fatal("expected interaction to be allowed after unlock")
	}
}

func TestUserManagerCreateAssignAndLookup(t *testing.T) {
	um := &UserManager{
		users:        make(map[string]*User),
		deviceToUser: make(map[string]string),
	}

	um.CreateUser("user-1", "Alice", "#ff00ff")
	um.CreateUser("user-2", "Bob", "#00ffff")
	um.AssignDevice("device-a", "user-1")

	user := um.GetUserForDevice("device-a")
	if user == nil {
		t.Fatal("expected assigned device lookup to return a user")
	}
	if user.ID != "user-1" || user.Name != "Alice" || user.CursorColor != "#ff00ff" {
		t.Fatalf("unexpected user lookup result: %+v", *user)
	}

	if missing := um.GetUserForDevice("missing-device"); missing != nil {
		t.Fatalf("expected nil for missing device, got %+v", *missing)
	}
}
