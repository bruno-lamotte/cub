#ifndef CUB_CONSTANTS_H
# define CUB_CONSTANTS_H

# define MSTR_PATROL_MIN_DIST       2.0
# define MSTR_PATROL_MAX_DIST       10.0
# define MSTR_PATROL_CLOSE_LIMIT    8.0
# define MSTR_SCORE_BOTTLENECK      2
# define MSTR_SCORE_BAD_DIST        3
# define MSTR_SCORE_BASE            10
# define MSTR_SCORE_NEIGHBOR_MULT   5
# define MSTR_SCORE_CLOSE_BONUS     5
# define MSTR_PATROL_SEED_X         17
# define MSTR_PATROL_SEED_Y         31

# define MSTR_ALERT_PATROL_TIMEOUT  300
# define MSTR_STATION_TIMEOUT       180
# define MSTR_ALERT_TIMEOUT         120
# define MSTR_CHASE_LOST_LOS_TIMEOUT 120
# define MSTR_SCAN_TIMEOUT          90
# define ALARM_DISARM_TIME          120
# define MSTR_DEBUG_PRINT_FREQ      60

# define MSTR_COLLISION_DIST_SQ     0.36     /*  (0.6 tiles) */
# define MSTR_LIGHT_PROX_DIST_SQ    0.64     /* (0.8 tiles) */
# define MSTR_DEST_REACHED_DIST_SQ  0.02     /* (~0.14 tiles) */
# define MSTR_ROTATION_STEP         0.04
# define MSTR_CHASE_SPEED           0.035
# define MSTR_PATROL_SPEED          0.015     /* Speed of the monster during patrol */
# define MSTR_MELEE_DIST_SQ         0.81     /* (0.9 tiles) */
# define MSTR_ATTACK_CHASE_DIST_SQ  1.44     /* (1.2 tiles) */

# define PLAYER_DET_LIGHT_MAX_DIST_SQ 60.0f  /* (10 tiles) */
# define PLAYER_DET_DARK_MAX_DIST_SQ  4.0f   /* (2 tiles) */
# define PLAYER_DET_CLOSE_DIST_SQ   1.00f    /* (1 tile) */

# define MSTR_ATTACK_DURATION       40       /* Duration of the attack state in frames */
# define MSTR_ATTACK_DAMAGE_FRAME   20       /* Frame at which the damage is applied */
# define MSTR_ATTACK_DAMAGE         20       /* Damage inflicted to the player */
# define PLAYER_MAX_HP              100      /* Maximum player HP */

/* Monster animation frames & speed divisors (ticks per animation frame) */
# define MSTR_PATROL_FRAMES          48
# define MSTR_COURSE_FRAMES          21
# define MSTR_PUNCH_FRAMES           26

# define MSTR_PATROL_SPEED_DIV       1
# define MSTR_COURSE_SPEED_DIV       2
# define MSTR_PUNCH_SPEED_DIV        2

# define MONSTER_SCALE               1.8
# define MSTR_ORIG_WIDTH             1920.0
# define MSTR_ORIG_HEIGHT            1080.0

#endif
