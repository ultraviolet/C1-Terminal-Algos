import gamelib
import random
import math
import warnings
from sys import maxsize
import json


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
        """ 
        Read in config and perform any initial setup here 
        """
        gamelib.debug_write('Configuring your custom algo strategy...')
        self.config = config
        global WALL, SUPPORT, TURRET, SCOUT, DEMOLISHER, INTERCEPTOR, MP, SP
        WALL = config["unitInformation"][0]["shorthand"]
        SUPPORT = config["unitInformation"][1]["shorthand"]
        TURRET = config["unitInformation"][2]["shorthand"]
        SCOUT = config["unitInformation"][3]["shorthand"]
        DEMOLISHER = config["unitInformation"][4]["shorthand"]
        INTERCEPTOR = config["unitInformation"][5]["shorthand"]
        MP = 1
        SP = 0
        # This is a good place to do initial setup
        self.state = []

    def on_turn(self, turn_state):
        """
        This function is called every turn with the game state wrapper as
        an argument. The wrapper stores the state of the arena and has methods
        for querying its state, allocating your current resources as planned
        unit deployments, and transmitting your intended deployments to the
        game engine.
        """

        self.state = json.loads(turn_state)
        
        game_state = gamelib.GameState(self.config, turn_state)
        game_state.attempt_spawn(DEMOLISHER, [24, 10], 3)
        gamelib.debug_write('Performing turn {} of your custom algo strategy'.format(game_state.turn_number))
        game_state.suppress_warnings(True)  #Comment or remove this line to enable warnings.

        self.starter_strategy(game_state)

        game_state.submit_turn()


    """
    NOTE: All the methods after this point are part of the sample starter-algo
    strategy and can safely be replaced for your custom algo.
    """

    def starter_strategy(self, game_state):
        """
        For defense we will use a spread out layout and some interceptors early on.
        We will place turrets near locations the opponent managed to score on.
        For offense we will use long range demolishers if they place stationary units near the enemy's front.
        If there are no stationary units to attack in the front, we will send Scouts to try and score quickly.
        """
        # First, place basic defenses
        self.build_defences(game_state)
        
        # Now build reactive defenses based on where the enemy scored


        # If the turn is less than 5, stall with interceptors and wait to see enemy's base
        if game_state.turn_number < 5:
            self.stall_with_interceptors(game_state)
        else:
            # Now let's analyze the enemy base to see where their defenses are concentrated.
            # If they have many units in the front we can build a line for our demolishers to attack them at long range.
            if self.detect_enemy_unit(game_state, unit_type=None, valid_x=None, valid_y=[14, 15]) > 10:
                self.demolisher_line_strategy(game_state)
            else:
                # They don't have many units in the front so lets figure out their least defended area and send Scouts there.

                # Only spawn Scouts every other turn
                # Sending more at once is better since attacks can only hit a single scout at a time
                if game_state.turn_number % 2 == 1:
                    # To simplify we will just check sending them from back left and right
                    scout_spawn_location_options = [[13, 0], [14, 0]]
                    best_location = self.least_damage_spawn_location(game_state, scout_spawn_location_options)
                    game_state.attempt_spawn(SCOUT, best_location, 1000)

                # Lastly, if we have spare SP, let's build some supports
                support_locations = [[13, 2], [14, 2], [13, 3], [14, 3]]
                game_state.attempt_spawn(SUPPORT, support_locations)

    def build_primary_defence(self, game_state):
        """
        Build basic defenses using hardcoded locations.
        Remember to defend corners and avoid placing units in the front where enemy demolishers can attack them.
        """
        # Useful tool for setting up your base locations: https://www.kevinbai.design/terminal-map-maker
        # More community tools available at: https://terminal.c1games.com/rules#Download

        # Place turrets that attack enemy units
        turret_locations = [[3, 12], [24, 12], [10, 11], [17, 11]]
        # attempt_spawn will try to spawn units if we have resources, and will check if a blocking unit is already there
        game_state.attempt_spawn(TURRET, turret_locations)
        # upgrade turrets so they are more powerful
        game_state.attempt_upgrade(turret_locations)
        
        # Place walls in front of turrets to soak up damage for them
        wall_locations = [[2, 13], [3, 13], [4, 11], 
                          [23, 13], [24, 13], [25, 13],
                          [9, 12], [10, 12], [11, 12],
                          [16, 12], [17, 12], [18, 12]]
        game_state.attempt_spawn(WALL, wall_locations)

    def build_secondary_defence(self, game_state):
        wall_locations = [[[0, 13], [1, 13], [2, 13], [3, 13], [4, 13], [23, 13], [24, 13], [25, 13], [26, 13], [27, 13], [5, 12], [6, 12], [7, 12], [8, 12], [9, 12], [10, 12], [11, 12], [16, 12], [17, 12], [18, 12], [19, 12], [20, 12], [21, 12], [22, 12], [12, 11], [13, 11], [14, 11], [15, 11]]]
        game_state.attempt_spawn(WALL, wall_locations)

        # turret_locations = [[3, 12], [24, 12], [6, 11], [10, 11], [17, 11], [21, 11]]
        # for i in range(0, len(turret_locations)-1):
        #     if(game_state.get_resource(0) >= 6):
        #         game_state.attempt_spawn(TURRET, turret_locations[i])
        #         game_state.attempt_upgrade(turret_locations[i])
        #     else:
        #         break

    def build_support(self, game_state):
        support_locations = [[12, 7], [13, 7], [14, 7], [15, 7], [12, 6], [13, 6], [14, 6], [15, 6]]
        
        for i in range(0, len(support_locations)-1):
            if(game_state.get_resource(0) >= 8):
                game_state.attempt_spawn(SUPPORT, support_locations[i])
                game_state.attempt_upgrade(support_locations[i])
            else:
                break


    def build_defences(self, game_state):
        """
        Build basic defenses using hardcoded locations.
        Remember to defend corners and avoid placing units in the front where enemy demolishers can attack them.
        """
        # Useful tool for setting up your base locations: https://www.kevinbai.design/terminal-map-maker
        # More community tools available at: https://terminal.c1games.com/rules#Download

        # Place turrets that attack enemy units
        a = self.def_funnel(game_state, 12, 13, 1, 15, 1, True)
        gamelib.debug_write(a)

    def build_reactive_defense(self, game_state):
        """
        This function builds reactive defenses based on where the enemy scored on us from.
        We can track where the opponent scored by looking at events in action frames 
        as shown in the on_action_frame function
        """
        for location in self.scored_on_locations:
            # Build turret one space above so that it doesn't block our own edge spawn locations
            build_location = [location[0], location[1]+1]
            game_state.attempt_spawn(TURRET, build_location)

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

        #self.state =  json.loads(turn_string)

        """
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
    
     
       """


    #FUNCTIONS BELOW


    def def_frontline(self, game_state, l, r, override=False, upgrade=[0,1,2]):
        wall_locations = [[0, 13], [1, 13], [2, 13], [3, 13], [4, 13], [23, 13], [24, 13], [25, 13], [26, 13], [27, 13], [5, 12], [6, 12], [7, 12], [8, 12], [9, 12], [10, 12], [11, 12], [16, 12], [17, 12], [18, 12], [19, 12], [20, 12], [21, 12], [22, 12], [12, 11], [13, 11], [14, 11], [15, 11], [2, 13], [3, 13], [4, 11], 
                          [23, 13], [24, 13], [25, 13],
                          [9, 12], [10, 12], [11, 12],
                          [16, 12], [17, 12], [18, 12]]
       

        turret_locations = [[3, 12], [24, 12], [10, 11], [17, 11]]
        

        support_locations = [[12, 7], [13, 7], [14, 7], [15, 7], [12, 6], [13, 6], [14, 6], [15, 6]]

        res = [self.util_clean_place(wall_locations), self.util_clean_place(turret_locations), self.util_clean_place(support_locations)]
        
        if override:
            for i in res:
                if i == 1 or i == 2:
                    return i
        

        for e in wall_locations:
            if not (i[0] >= l and i[0] <= r):
                    wall_locations.remove(e)
        
        for e in turret_locations:
            if not (i[0] >= l and i[0] <= r):
                    turret_locations.remove(e)
        
        for e in turret_locations:
            if not (i[0] >= l and i[0] <= r):
                turret_locations.remove(e)

        for i in range(0, len(support_locations)-1):
            if(game_state.get_resource(0) >= 8):
                game_state.attempt_spawn(SUPPORT, support_locations[i])
                game_state.attempt_upgrade(support_locations[i])
            else:
                break
        

        



        

    """
    self
    game_state = game_state object
    x = xpos
    y = ypos
    width = width of funnel
    length = depth of funnel
    direction = [-1 for leftwards facing, 1 for rightwards facing]

    return [0 = successful, 1 = invalid dimensions, 2 = not enough troops]
    """
    def def_funnel(self, game_state, x, y, width, length, direction, override=False):

        x1 = x-width
        y1 = y
        x2 = x+width
        y2 = y


        arr = []
        for i in range(length):
            arr.append([x1+ direction * (i+1), y1 -(i+1)])
            arr.append([x2+ direction * (i+1), y2 -(i+1)])
        
        res = self.util_clean_place(arr, override)

        if res == 1:
            return 1
        
        if self.util_can_place('WALL', res[0]):
            return 2
        
        game_state.attempt_spawn(WALL, res[1])
        return 0
        
    
    def util_handle_placing(self, game_state, arr, override, upgrade):

        res = [self.util_clean_place(arr[0], override), self.util_clean_place(arr[1], override), self.util_clean_place(arr[2], override)]
        cor = [WALL, TURRET, SUPPORT]
        
        for e in res:
            if e == 1:
                return e
        
        if not override:
            for i in range(len(res)):
                if upgrade[i] == 0:
                    game_state.attempt_spawn(cor[i], res[i])
                elif upgrade[i] == 1:
                    game_state.attempt_spawn(cor[i], res[i])
                    game_state.attempt_upgrade(res[i])
                else:
                    pass
        

    """
    self
    arr = array of locations
    override (Default False)

    Returns 1 as an error code if not overriden
    Otherwise, cleanses array of bound errors and existing towers
    """
    def util_clean_place(self, arr, override=False):
        units = []
        for e in self.state["p1Units"]:
            for i in e:
                units.append([int(i[0]), int(i[1])])
        
        total = len(arr)
        for e in arr:
            if e in units:
                arr.remove(e)
            if not self.util_bounds_ours(e[0], e[1]):
                if not override:
                    return 1
                else:
                    arr.remove(e)
        
        return [total, arr]

    """
    self
    str = type of building
    amount = amount you want to place

    return = [True if you can place the amount of buildings, False i.o]
    """
    def util_can_place(self, str, amount, upgrade=False):
        if str == 'WALL':
            return int(self.state["p1Stats"][1]) < (amount + int(upgrade)) 
        elif str == 'SUPPORT':
            return int(self.state["p1Stats"][1]) < (amount + 4*int(upgrade))/4
        elif str == 'TURRENT':
            return int(self.state["p1Stats"][1]) < (amount+4*int(upgrade))/2

    """
    self
    x = xpos
    y = ypos

    return [True if (x, y) is in bounds of our area, False if (x, y) is out of bounds of our area]
    """
    def util_bounds_ours(self, x, y):
        if y in range(14) and x in range(28):
            if x >= 13-y and x <= 14+y:
                return True 
        return False

    

        

if __name__ == "__main__":
    algo = AlgoStrategy()
    algo.start()
