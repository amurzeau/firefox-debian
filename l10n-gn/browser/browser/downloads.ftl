# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## The title and aria-label attributes are used by screen readers to describe
## the Downloads Panel.

downloads-window =
    .title = Ñemboguejy
downloads-panel =
    .aria-label = Ñemboguejy

##

# The style attribute has the width of the Downloads Panel expressed using
# a CSS unit. The longest labels that should fit are usually those of
# in-progress and blocked downloads.
downloads-panel-list =
    .style = width: 70ch
# The style attribute has the width of the Downloads Panel expressed using
# a CSS unit. The longest labels that should fit are usually those of
# in-progress and blocked downloads.
downloads-panel-items =
    .style = width: 45em
downloads-cmd-pause =
    .label = Mombyta
    .accesskey = M
downloads-cmd-resume =
    .label = Ehorei hese
    .accesskey = E
downloads-cmd-cancel =
    .tooltiptext = Heja
downloads-cmd-cancel-panel =
    .aria-label = Heja
# This message is only displayed on Windows and Linux devices
downloads-cmd-show-menuitem =
    .label = Ñongatuha Ryrúpe Jeike
    .accesskey = Ñ
# This message is only displayed on macOS devices
downloads-cmd-show-menuitem-mac =
    .label = Ehechauka Finder-pe
    .accesskey = F
downloads-cmd-show-menuitem-2 =
    .label =
        { PLATFORM() ->
            [macos] Ehechauka Finder-pe
           *[other] Ehechauka marandurendápe
        }
    .accesskey = F
downloads-cmd-use-system-default =
    .label = Embojuruja apopyvusu rechahápe
    .accesskey = V
# We can use the same accesskey as downloads-cmd-always-open-similar-files.
# Both should not be visible in the downloads context menu at the same time.
downloads-cmd-always-use-system-default =
    .label = Embojuruja tapia apopyvusu rechahápe
    .accesskey = w
downloads-cmd-show-button =
    .tooltiptext =
        { PLATFORM() ->
            [macos] Ehechauka Finder-pe
           *[other] Ñongatuha Ryrúpe Jeike
        }
downloads-cmd-show-panel =
    .aria-label =
        { PLATFORM() ->
            [macos] Ehechauka Finder-pe
           *[other] Ñongatuha Ryrúpe Jeike
        }
downloads-cmd-show-description =
    .value =
        { PLATFORM() ->
            [macos] Ehechauka Finder-pe
           *[other] Ñongatuha Ryrúpe Jeike
        }
# We can use the same accesskey as downloads-cmd-always-use-system-default.
# Both should not be visible in the downloads context menu at the same time.
downloads-cmd-always-open-similar-files =
    .label = Embojuruja tapiaite marandurenda ojuehegua
    .accesskey = w
downloads-cmd-show-button-2 =
    .tooltiptext =
        { PLATFORM() ->
            [macos] Ehechauka Finder-pe
           *[other] Ehechauka marandurendápe
        }
downloads-cmd-show-panel-2 =
    .aria-label =
        { PLATFORM() ->
            [macos] Ehechauka Finder-pe
           *[other] Ehechauka marandurendápe
        }
downloads-cmd-show-description-2 =
    .value =
        { PLATFORM() ->
            [macos] Ehechauka Finder-pe
           *[other] Ehechauka marandurendápe
        }
downloads-cmd-show-downloads =
    .label = Ñemboguejykuéra Ñongatuha Hechauka
downloads-cmd-retry =
    .tooltiptext = Eñeha’ãjey
downloads-cmd-retry-panel =
    .aria-label = Eñeha’ãjey
downloads-cmd-go-to-download-page =
    .label = Kuatiarogue Omboguejývape Jeho
    .accesskey = J
downloads-cmd-copy-download-link =
    .label = Ñemboguejy rape mbohasarã
    .accesskey = m
downloads-cmd-remove-from-history =
    .label = Emboguete tembiasakuégui
    .accesskey = m
downloads-cmd-clear-list =
    .label = Embogue jeike hague ra’ãngarupa
    .accesskey = a
downloads-cmd-clear-downloads =
    .label = Mboguejypyre mopotĩ
    .accesskey = M
# This command is shown in the context menu when downloads are blocked.
downloads-cmd-unblock =
    .label = Emoneĩ ñemboguejy
    .accesskey = o
# This is the tooltip of the action button shown when malware is blocked.
downloads-cmd-remove-file =
    .tooltiptext = Marandurenda mboguete
downloads-cmd-remove-file-panel =
    .aria-label = Marandurenda mboguete
# This is the tooltip of the action button shown when potentially unwanted
# downloads are blocked. This opens a dialog where the user can choose
# whether to unblock or remove the download. Removing is the default option.
downloads-cmd-choose-unblock =
    .tooltiptext = Emboguete Marandurenda térã Emoneĩ Ñemboguejy
downloads-cmd-choose-unblock-panel =
    .aria-label = Emboguete Marandurenda térã Emoneĩ Ñemboguejy
# This is the tooltip of the action button shown when uncommon downloads are
# blocked.This opens a dialog where the user can choose whether to open the
# file or remove the download. Opening is the default option.
downloads-cmd-choose-open =
    .tooltiptext = Eike térã eipe’a Marandurenda
downloads-cmd-choose-open-panel =
    .aria-label = Eike térã eipe’a Marandurenda
# Displayed when hovering a blocked download, indicates that it's possible to
# show more information for user to take the next action.
downloads-show-more-information =
    .value = Maranduve jehechauka
# Displayed when hovering a complete download, indicates that it's possible to
# open the file using an app available in the system.
downloads-open-file =
    .value = Marandurendápe jeike

## Displayed when the user clicked on a download in process. Indicates that the
## downloading file will be opened after certain amount of time using an app
## available in the system.
## Variables:
##   $hours (number) - Amount of hours left till the file opens.
##   $seconds (number) - Amount of seconds left till the file opens.
##   $minutes (number) - Amount of minutes till the file opens.

downloading-file-opens-in-hours-and-minutes = Ijurujáta { $hours }h { $minutes }m…
downloading-file-opens-in-minutes = Ijurujáta { $minutes }m…
downloading-file-opens-in-minutes-and-seconds = Ijurujáta { $minutes } { $seconds }s…
downloading-file-opens-in-seconds = Ijurujáta { $seconds }s…
downloading-file-opens-in-some-time = Ijurujáta henyhẽte vove…

##

# Displayed when hovering a download which is able to be retried by users,
# indicates that it's possible to download this file again.
downloads-retry-download =
    .value = Eha’ãjey ñemboguejy
# Displayed when hovering a download which is able to be cancelled by users,
# indicates that it's possible to cancel and stop the download.
downloads-cancel-download =
    .value = Eheja ñemboguejy
# This string is shown at the bottom of the Downloads Panel when all the
# downloads fit in the available space, or when there are no downloads in
# the panel at all.
downloads-history =
    .label = Opaite ñemboguejypyre jehechauka
    .accesskey = S
# This string is shown at the top of the Download Details Panel, to indicate
# that we are showing the details of a single download.
downloads-details =
    .title = Ñemboguejy Sa’ikue
downloads-clear-downloads-button =
    .label = Mboguejypyre mopotĩ
    .tooltiptext = Emopotĩ oñemboguejypámava, ojehejareipyréva ha ndoguejyporãiva'ekue
# This string is shown when there are no items in the Downloads view, when it
# is displayed inside a browser tab.
downloads-list-empty =
    .value = Ndaipóri mboguejypyre.
# This string is shown when there are no items in the Downloads Panel.
downloads-panel-empty =
    .value = Ndaipóri ñemboguejypyre ko tembiapopa’ũme.
# This is displayed in an item at the bottom of the Downloads Panel when there
# are more downloads than can fit in the list in the panel.
#   $count (number) - number of files being downloaded that are not shown in the
#                     panel list.
downloads-more-downloading =
    { $count ->
        [one] { $count } emboguejyve marandurenda
       *[other] { $count } emboguejyve marandurenda
    }
