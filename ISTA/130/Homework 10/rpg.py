"""
Christopher Chapline
Homework 10
ISTA 130
Adam Stevens
"""
from random import randint

class Fighter():
    """
    A fighter is a character in an RPG who can battle against other
    fighters
    """

    def __init__(self, name, hit_points=10):
        """
        Creates a new Fighter object.

        Keyword arguments:
        name -- The name of this fighter
        hit_points -- The amount of health this fighter has (default 10)
        """
        self.name = name
        self.hit_points = hit_points

    def __repr__(self):
        """Returns a string representation of this figher"""
        return self.name + " (HP: " + str(self.hit_points) + ")"

    def take_damage(self, damage_amount):
        """
        Removes the specified amount of health from the fighter's hitpoints and
        prints a message about the fighter's state (alive or dead).

        Keyword arguments:
        damage_amount -- The amount of damage dealt to this fighter
        """
        # Deal damage
        self.hit_points -= damage_amount

        # Determine the state of the fighter
        if self.hit_points <= 0:
            print( "\tAlas,", self.name, "has fallen!" )

        else:
            print( "\t" + self.name, "has", self.hit_points, "hit points remaining." )

    def attack(self, other_fighter):
        """
        Attacks the specified fighter for a random amount of damage

        Keyword arguments:
        other_fighter -- The fighter that this fighter will be dealing damage to
        """
        # Print out information about who is attacking who
        print( self.name, "attacks", other_fighter.name + "!" )

        # Determine the nature of the attack
        attack_was_successful = randint(0, 20) >= 12

        # Perform the attack
        if attack_was_successful:
            damage_dealt = randint(1, 6)
            print( "\tHits for", damage_dealt, "hit points!" )
            other_fighter.take_damage( damage_dealt )

        else:
            print( "\tMisses" )

    def is_alive(self):
        """Returns true if this fighter is still alive (hp > 0)"""
        return self.hit_points > 0

def combat_round( fighter_one, fighter_two ):
    """
    Simulates combat between two fighers

    Keyword arguments:
    fighter_one -- One of the fighters engaging in combat
    fighter_two -- The other fighter engaging in combat
    """
    # Determine who is dealing damage
    fighter_one_chance = randint(1, 6)
    fighter_two_chance = randint(1, 6)

    # Determine who is dealing damage
    if fighter_one_chance == fighter_two_chance:
        print( "Simultaneous!" )
        fighter_one.attack( fighter_two )
        fighter_two.attack( fighter_one )

    elif fighter_one_chance > fighter_two_chance:
        fighter_one.attack( fighter_two )

    else:
        fighter_two.attack( fighter_one )

def main():
    """
    Simulates combat between two fighters
    """
    # Create our fighters
    fighter_one = Fighter("Leeroy Jenkins")
    fighter_two = Fighter("Mitsurugi")
    current_round = 1

    while fighter_one.is_alive() and fighter_two.is_alive():
        # Print out the round header
        message = " Round " + str(current_round) + " "
        print( message.center(35, "=" ) )

        # Print out information about the fighters
        print( fighter_one )
        print( fighter_two )

        # Simulate combat
        combat_round( fighter_one, fighter_two )

        # Pause input
        input( "Enter to fight!" )
        current_round += 1
        print()

    # Print end of round information
    print( "The battle is over!")
    print( fighter_one )
    print( fighter_two )

if __name__ == '__main__':
    main()
