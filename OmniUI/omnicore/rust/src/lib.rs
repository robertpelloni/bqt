#[cxx::bridge]
mod ffi {
    // Define the C++ types we want to interact with
    unsafe extern "C++" {
        include!("OmniRustBridge.h");
    }

    // Expose Rust functions to C++
    extern "Rust" {
        fn register_device_safe(device_id: String, name: String, device_type: String) -> bool;
        fn update_cursor_safe(device_id: String, x: f64, y: f64) -> bool;
        fn get_device_count() -> i32;
    }
}

use std::sync::Mutex;
use std::collections::HashMap;
use lazy_static::lazy_static;

struct DeviceState {
    name: String,
    device_type: String,
    x: f64,
    y: f64,
}

lazy_static! {
    // Rust's Mutex guarantees absolute memory safety. 
    // The C++ UI thread CANNOT access this without locking it at the compiler level.
    static ref GLOBAL_STATE: Mutex<HashMap<String, DeviceState>> = Mutex::new(HashMap::new());
}

fn register_device_safe(device_id: String, name: String, device_type: String) -> bool {
    let mut state = GLOBAL_STATE.lock().unwrap();
    
    if state.contains_key(&device_id) {
        return false;
    }

    state.insert(device_id, DeviceState {
        name,
        device_type,
        x: 0.0,
        y: 0.0,
    });

    true
}

fn update_cursor_safe(device_id: String, x: f64, y: f64) -> bool {
    let mut state = GLOBAL_STATE.lock().unwrap();
    
    if let Some(device) = state.get_mut(&device_id) {
        // Enforce absolute OS-level boundary checks strictly inside Rust
        let clamped_x = x.clamp(0.0, 7680.0); // Max 8K resolution boundary
        let clamped_y = y.clamp(0.0, 4320.0);
        
        device.x = clamped_x;
        device.y = clamped_y;
        return true;
    }
    
    false
}

fn get_device_count() -> i32 {
    let state = GLOBAL_STATE.lock().unwrap();
    state.len() as i32
}
