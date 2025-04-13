package com.waterlevel.waterlevel;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class WaterlevelApplication {

	public static void main(String[] args) {

		System.out.println("Water Level Application Started");
		SpringApplication.run(WaterlevelApplication.class, args);
	}

}
