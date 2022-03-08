use rustc_version::{version, Version};

fn main() {
    if version().unwrap() < Version::parse("1.57.0-alpha").unwrap() {
        println!("cargo:rustc-cfg=try_reserve");
    }
}
