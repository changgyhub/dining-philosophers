# Dining-Philosophers
A simple yet robust solution to Dining Philosophers in pthread

### Output format
| Philo | State       | Fork   | Held by  |
|-------|-------------|--------|----------|
| [ 0]: | Thinking    | [ 0]:  | Free     |
| [ 1]: | Eating      | [ 1]:  | 1        |
| [ 2]: | Waiting     | [ 2]:  | 1        |
| [ 3]: | Terminated  | [ 3]:  | 2        |
| [ 4]: | Eating      | [ 4]:  | 4        |
| [ 5]: | Thinking    | [ 5]:  | 4        |
| [ 6]: | Thinking    | [ 6]:  | Free     |
| [ 7]: | Thinking    | [ 7]:  | Free     |
| [ 8]: | Eating      | [ 8]:  | 8        |
| [ 9]: | Thinking    | [ 9]:  | 8        |
| Th= 5 | Wa= 1 Ea= 3 | Use= 7 | Avail= 3 |

### Usage
 - Compilation: ` gcc DPP.c -o DPP -Wall -pthread`
 - Run: `./DPP <N> <S> <T>`<br>
   N = number of philosophers<br>
   S = random number seed<br>
   T = seconds of running<br>
