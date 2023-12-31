import gamelib
import random
import math
import warnings
from sys import maxsize
import json

import ai_env

"""
Most of the algo code you write will be in this file unless you create new
modules yourself. Start by modifying the 'on_turn' function.

Advanced strategy tips: 

  - You can analyze action frames by modifying on_action_frame function

  - The GameState.map object can be manually manipulated to create hypothetical 
  board states. Though, we recommended making a copy of the map to preserve 
  the actual current map state.
"""

class AlgoStrategy(gamelib.AlgoCore):
    def __init__(self):
        super().__init__()
        seed = random.randrange(maxsize)
        random.seed(seed)
        gamelib.debug_write('Random seed: {}'.format(seed))

    def on_game_start(self, config):

        gamelib.debug_write('Configuring your custom algo strategy...')
        self.config = config
        global WALL, SUPPORT, TURRET, SCOUT, DEMOLISHER, INTERCEPTOR, MP, SP
        WALL = config["unitInformation"][0]["shorthand"] # FF
        SUPPORT = config["unitInformation"][1]["shorthand"] # EF
        TURRET = config["unitInformation"][2]["shorthand"] # DF
        SCOUT = config["unitInformation"][3]["shorthand"]  # PI
        DEMOLISHER = config["unitInformation"][4]["shorthand"] # EI
        INTERCEPTOR = config["unitInformation"][5]["shorthand"] # SI
        # RM -> MARKED TO REMOVE
        # UP -> MARKED TO UPGRADE

        # gamelib.debug_write(config)
        for i in config["unitInformation"]:
            gamelib.debug_write(i["shorthand"])

        MP = 1
        SP = 0
        # This is a good place to do initial setup
        self.scored_on_locations = []


        """
        1. Create the environment
        """

    def on_turn(self, turn_state):
        #wait for ai_env to say the action

        """
        while action == null:
            time.sleep(0.1)
        
        parse action
        actions[action]()

        game_state.submit_turn()
        """

        game_state = gamelib.GameState(self.config, turn_state)
        gamelib.debug_write(ai_env.game_state_to_observation(turn_state,game_state))
        gamelib.debug_write('Performing turn {} of your custom algo strategy'.format(game_state.turn_number))
        game_state.suppress_warnings(True)  #Comment or remove this line to enable warnings.

        self.strategy(game_state)
        game_state.submit_turn()


    """
    NOTE: All the methods after this point are part of the sample starter-algo
    strategy and can safely be replaced for your custom algo.
    """

    def strategy(self, game_state):
        """
        For defense we will use a spread out layout and some interceptors early on.
        We will place turrets near locations the opponent managed to score on.
        For offense we will use long range demolishers if they place stationary units near the enemy's front.
        If there are no stationary units to attack in the front, we will send Scouts to try and score quickly.
        """
        # First, place basic defenses
        
        self.build_front_line(game_state)

        game_state.submit_turn()
        


    def build_front_line(self, game_state):
        turret_locations = [[3, 12], [24, 12], [10, 11], [17, 11]]
        game_state.attempt_spawn(TURRET, turret_locations)
        game_state.attempt_upgrade(turret_locations)
        
        wall_locations = [[2, 13], [3, 13], [4, 11], 
                          [23, 13], [24, 13], [25, 13],
                          [9, 12], [10, 12], [11, 12],
                          [16, 12], [17, 12], [18, 12]]
        game_state.attempt_spawn(WALL, wall_locations)

        wall_locations = [[0, 13], [1, 13], [2, 13], [3, 13], [4, 13], [23, 13], [24, 13], [25, 13], [26, 13], [27, 13], [5, 12], [6, 12], [7, 12], [8, 12], [9, 12], [10, 12], [11, 12], [16, 12], [17, 12], [18, 12], [19, 12], [20, 12], [21, 12], [22, 12], [12, 11], [13, 11], [14, 11], [15, 11], [8, 10], [19, 10]]
        game_state.attempt_spawn(WALL, wall_locations)


    def build_support(self, game_state, num):
        support_locations = [[12, 7], [13, 7], [14, 7], [15, 7], [12, 6], [13, 6], [14, 6], [15, 6]]
        
        amount_placed = 0
        for i in range(0, support_locations):
            if game_state.can_spawn(SUPPORT, support_locations[i]):
               if game_state.get_resource(0) >= 8:
                    game_state.attempt_spawn(SUPPORT, support_locations[i])
                    game_state.attempt_upgrade(support_locations[i])
                    amount_placed +=1
            if amount_placed == num:
                break

        if amount_placed == num:
            return 0
        else:
            return -1
    
    def place_turrets(id):
        
        if not len(id) == 4:
            return -100

    def stall_with_interceptors(self, game_state):
        """
        Send out interceptors at random locations to defend our base from enemy moving units.
        """
        # We can spawn moving units on our edges so a list of all our edge locations
        friendly_edges = game_state.game_map.get_edge_locations(game_state.game_map.BOTTOM_LEFT) + game_state.game_map.get_edge_locations(game_state.game_map.BOTTOM_RIGHT)
        
        # Remove locations that are blocked by our own structures 
        # since we can't deploy units there.
        deploy_locations = self.filter_blocked_locations(friendly_edges, game_state)
        
        # While we have remaining MP to spend lets send out interceptors randomly.
        while game_state.get_resource(MP) >= game_state.type_cost(INTERCEPTOR)[MP] and len(deploy_locations) > 0:
            # Choose a random deploy location.
            deploy_index = random.randint(0, len(deploy_locations) - 1)
            deploy_location = deploy_locations[deploy_index]
            
            game_state.attempt_spawn(INTERCEPTOR, deploy_location)
            """
            We don't have to remove the location since multiple mobile 
            units can occupy the same space.
            """

    def demolisher_line_strategy(self, game_state):
        """
        Build a line of the cheapest stationary unit so our demolisher can attack from long range.
        """
        # First let's figure out the cheapest unit
        # We could just check the game rules, but this demonstrates how to use the GameUnit class
        stationary_units = [WALL, TURRET, SUPPORT]
        cheapest_unit = WALL
        for unit in stationary_units:
            unit_class = gamelib.GameUnit(unit, game_state.config)
            if unit_class.cost[game_state.MP] < gamelib.GameUnit(cheapest_unit, game_state.config).cost[game_state.MP]:
                cheapest_unit = unit

        # Now let's build out a line of stationary units. This will prevent our demolisher from running into the enemy base.
        # Instead they will stay at the perfect distance to attack the front two rows of the enemy base.
        for x in range(27, 5, -1):
            game_state.attempt_spawn(cheapest_unit, [x, 11])

        # Now spawn demolishers next to the line
        # By asking attempt_spawn to spawn 1000 units, it will essentially spawn as many as we have resources for
        game_state.attempt_spawn(DEMOLISHER, [24, 10], 1000)

    def least_damage_spawn_location(self, game_state, location_options):
        """
        This function will help us guess which location is the safest to spawn moving units from.
        It gets the path the unit will take then checks locations on that path to 
        estimate the path's damage risk.
        """
        damages = []
        # Get the damage estimate each path will take
        for location in location_options:
            path = game_state.find_path_to_edge(location)
            damage = 0
            for path_location in path:
                # Get number of enemy turrets that can attack each location and multiply by turret damage
                damage += len(game_state.get_attackers(path_location, 0)) * gamelib.GameUnit(TURRET, game_state.config).damage_i
            damages.append(damage)
        
        # Now just return the location that takes the least damage
        return location_options[damages.index(min(damages))]

    def detect_enemy_unit(self, game_state, unit_type=None, valid_x = None, valid_y = None):
        total_units = 0
        for location in game_state.game_map:
            if game_state.contains_stationary_unit(location):
                for unit in game_state.game_map[location]:
                    if unit.player_index == 1 and (unit_type is None or unit.unit_type == unit_type) and (valid_x is None or location[0] in valid_x) and (valid_y is None or location[1] in valid_y):
                        total_units += 1
        return total_units
        
    def filter_blocked_locations(self, locations, game_state):
        filtered = []
        for location in locations:
            if not game_state.contains_stationary_unit(location):
                filtered.append(location)
        return filtered

    def on_action_frame(self, turn_string):
        """
        This is the action frame of the game. This function could be called 
        hundreds of times per turn and could slow the algo down so avoid putting slow code here.
        Processing the action frames is complicated so we only suggest it if you have time and experience.
        Full doc on format of a game frame at in json-docs.html in the root of the Starterkit.
        """
        # Let's record at what position we get scored on
        state = json.loads(turn_string)
        events = state["events"]
        breaches = events["breach"]
        for breach in breaches:
            location = breach[0]
            unit_owner_self = True if breach[4] == 1 else False
            # When parsing the frame data directly, 
            # 1 is integer for yourself, 2 is opponent (StarterKit code uses 0, 1 as player_index instead)
            if not unit_owner_self:
                gamelib.debug_write("Got scored on at: {}".format(location))
                self.scored_on_locations.append(location)
                gamelib.debug_write("All locations: {}".format(self.scored_on_locations))


if __name__ == "__main__":
    algo = AlgoStrategy()
    algo.start()