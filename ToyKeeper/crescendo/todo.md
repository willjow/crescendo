TODO
----

[debug]
  * occasionally, very weird things will break
    - most likely the same issue as here:
      https://budgetlightforum.com/comment/1100942#comment-1100942
    - looks like maybe the only guaranteed fix is to implement off-time sleep
      mode, in which case the MCU will still be on and no memory integrity
      should be lost


[first-priority features]
  * add support for convoy 5A linear fet driver
    (this is probably only really worthwhile if OTSM works with this driver;
    efficiency gains from PWM to constant current are probably not that great)
    - add flashyhedgehog's shenanigans for calculating PWM output
      - or don't, and just make sure output can be set properly
    - add hwdef and config

  * add support for off time sleep mode
    - maybe not necessarily worth the resources because this UI doesn't use any
      concept of "medium" presses
    - OTOH, this guarantees that we don't lose SRAM integrity because the MCU
      will be powered at all relevant times


[second-priority features]
  * round battcheck to nearest 0.05V instead of floored to next 0.1V?
    - just need to check for whether upper or lower bound is closer
    - actually maybe this is not worth the space
      - just do it anyway and hide it behind a preprocessor guard?

  * experiment with pulse frequency modulation

  * update manual to reflect any changes

--------------------------------------------------------------------------------

[old]
  * add support for off-time capacitor or off-time sleep mode to distinguish
    between long and short presses
    - https://github.com/alexvanh/basic_off_time_driver/blob/master/README.md
    - the above reports that relying on SRAM decay is not a reliable
      long-term solution...
    - my current understanding is that the flipping of bits after power is
      disconnected is essentially due to noise, but I'm unsure why this
      property of the SRAM should change over time... more reading is
      required
