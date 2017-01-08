-- A solution contains projects, and defines the available configurations
solution "Giffy"
   configurations { "Debug", "Release" }
 
   -- A project defines one build target
   project "Giffy"
      kind "ConsoleApp"
      language "C"
      files { "giffy.h", "giffy.c", "main.c" }
 
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" } 
