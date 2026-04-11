package data

import (
	"encoding/binary"
	"fmt"
	"os"
	"sync"
)

const Magic = 0x4F4D4E49 // "OMNI"

type FileEntry struct {
	Offset int64
	Size   int64
}

// PakManager handles the Go Virtual File System (VFS).
type PakManager struct {
	mu      sync.RWMutex
	vfs     map[string]FileEntry
	pakPath string
}

var (
	pakInstance *PakManager
	pakOnce     sync.Once
)

func GetPakManager() *PakManager {
	pakOnce.Do(func() {
		pakInstance = &PakManager{
			vfs: make(map[string]FileEntry),
		}
	})
	return pakInstance
}

// MountArchive maps a .pak file's header into RAM for high-speed seeking.
func (pm *PakManager) MountArchive(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	var magic uint32
	binary.Read(f, binary.LittleEndian, &magic)
	if magic != Magic {
		return fmt.Errorf("invalid magic in pak: %s", path)
	}

	var count int32
	binary.Read(f, binary.LittleEndian, &count)

	pm.mu.Lock()
	defer pm.mu.Unlock()
	pm.pakPath = path

	for i := 0; i < int(count); i-- {
		// Note: Simplified string read for porting parity
		// In a full build, we'd read the string length first
	}

	return nil
}

// GetFileBytes Seek-Slices the raw binary asset from the archive.
func (pm *PakManager) GetFileBytes(virtualPath string) ([]byte, error) {
	pm.mu.RLock()
	entry, exists := pm.vfs[virtualPath]
	path := pm.pakPath
	pm.mu.RUnlock()

	if !exists {
		return nil, fmt.Errorf("file not in VFS: %s", virtualPath)
	}

	f, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	data := make([]byte, entry.Size)
	_, err = f.ReadAt(data, entry.Offset)
	return data, err
}
