use std::fs::File;
use std::io::{self, BufRead, BufReader};
use std::time::Instant;

// 반복자 스타일 방식으로 "the" 개수 세기
fn search_with_iterator(filename: &str) -> io::Result<(usize, std::time::Duration)> {
    let file = File::open(filename)?;
    let reader = BufReader::new(file);
    let start = Instant::now();
    let mut count = 0;
    let search_term = "the";

    for line_result in reader.lines() {
        let line = line_result?;
        let lower_line = line.to_lowercase(); // 소문자로 변환

        // `matches`는 패턴에 해당하는 부분 문자열의 반복자를 반환합니다.
        // `count()`는 이 반복자의 요소 개수를 셉니다.
        count += lower_line.matches(search_term).count();
    }

    let duration = start.elapsed();
    Ok((count, duration))
}


fn main() {
    let filename = "The_Adventures_of_Sherlock_Holmes.txt";
    println!("Starting Rust benchmark...");

    // --- 반복자 방식 실행 ---
    match search_with_iterator(filename) {
        Ok((count, duration)) => {
            println!("[Rust Iterator Version]");
            println!("Found '{}' {} times.", "the", count);
            println!("Time taken: {:?}", duration);
        }
        Err(e) => {
            eprintln!("Error running iterator version: {}", e);
        }
    }
}