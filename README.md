# COSC315-Project 1
## October 5th, 2020

## Group members:
- Eric Achenbach
- Patrick Mahler
- Aaron Mahnic
- Nathan Wickenheiser

## Design choices:
- Due to the relatively small amount of coding required, we decided to all attempt and solve the project. We then ended up pushing Nathan's code to our master branch and collectively worked on it to try and resolve any remaining issues. 

## Issues with implementation:
- The code ran as expeted when executing the processes sequentially and in parallel. Sequential processes timeout properly. When running the `sleephello` script in parallel with a timeout value of `1`, not all child processes are killed during timeout (although a timeout is detected).
