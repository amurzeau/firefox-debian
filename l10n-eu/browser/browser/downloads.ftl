# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## The title and aria-label attributes are used by screen readers to describe
## the Downloads Panel.

downloads-window =
    .title = Deskargak
downloads-panel =
    .aria-label = Deskargak

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
    .style = width: 35em
downloads-cmd-pause =
    .label = Pausatu
    .accesskey = P
downloads-cmd-resume =
    .label = Berrekin
    .accesskey = r
downloads-cmd-cancel =
    .tooltiptext = Utzi
downloads-cmd-cancel-panel =
    .aria-label = Utzi
# This message is only displayed on Windows and Linux devices
downloads-cmd-show-menuitem =
    .label = Ireki dagoen karpeta
    .accesskey = k
# This message is only displayed on macOS devices
downloads-cmd-show-menuitem-mac =
    .label = Erakutsi Finder-en
    .accesskey = F
downloads-cmd-show-menuitem-2 =
    .label =
        { PLATFORM() ->
            [macos] Erakutsi Finder-en
           *[other] Erakutsi karpetan
        }
    .accesskey = k
downloads-cmd-use-system-default =
    .label = Ireki sistemaren ikustailean
    .accesskey = k
# We can use the same accesskey as downloads-cmd-always-open-similar-files.
# Both should not be visible in the downloads context menu at the same time.
downloads-cmd-always-use-system-default =
    .label = Ireki beti sistemaren ikustailean
    .accesskey = b
downloads-cmd-show-button =
    .tooltiptext =
        { PLATFORM() ->
            [macos] Erakutsi Finder-en
           *[other] Ireki dagoen karpeta
        }
downloads-cmd-show-panel =
    .aria-label =
        { PLATFORM() ->
            [macos] Erakutsi Finder-en
           *[other] Ireki dagoen karpeta
        }
downloads-cmd-show-description =
    .value =
        { PLATFORM() ->
            [macos] Erakutsi Finder-en
           *[other] Ireki dagoen karpeta
        }
# We can use the same accesskey as downloads-cmd-always-use-system-default.
# Both should not be visible in the downloads context menu at the same time.
downloads-cmd-always-open-similar-files =
    .label = Ireki beti antzerako fitxategiak
    .accesskey = b
downloads-cmd-show-button-2 =
    .tooltiptext =
        { PLATFORM() ->
            [macos] Erakutsi Finder-en
           *[other] Erakutsi karpetan
        }
downloads-cmd-show-panel-2 =
    .aria-label =
        { PLATFORM() ->
            [macos] Erakutsi Finder-en
           *[other] Erakutsi karpetan
        }
downloads-cmd-show-description-2 =
    .value =
        { PLATFORM() ->
            [macos] Erakutsi Finder-en
           *[other] Erakutsi karpetan
        }
downloads-cmd-show-downloads =
    .label = Erakutsi deskargen karpeta
downloads-cmd-retry =
    .tooltiptext = Saiatu berriro
downloads-cmd-retry-panel =
    .aria-label = Saiatu berriro
downloads-cmd-go-to-download-page =
    .label = Joan deskargaren orrira
    .accesskey = J
downloads-cmd-copy-download-link =
    .label = Kopiatu deskargaren lotura
    .accesskey = l
downloads-cmd-remove-from-history =
    .label = Kendu historiatik
    .accesskey = K
downloads-cmd-clear-list =
    .label = Garbitu aurrebista-panela
    .accesskey = a
downloads-cmd-clear-downloads =
    .label = Garbitu deskargak
    .accesskey = d
# This command is shown in the context menu when downloads are blocked.
downloads-cmd-unblock =
    .label = Baimendu deskarga
    .accesskey = B
# This is the tooltip of the action button shown when malware is blocked.
downloads-cmd-remove-file =
    .tooltiptext = Kendu fitxategia
downloads-cmd-remove-file-panel =
    .aria-label = Kendu fitxategia
# This is the tooltip of the action button shown when potentially unwanted
# downloads are blocked. This opens a dialog where the user can choose
# whether to unblock or remove the download. Removing is the default option.
downloads-cmd-choose-unblock =
    .tooltiptext = Kendu fitxategia edo baimendu deskarga
downloads-cmd-choose-unblock-panel =
    .aria-label = Kendu fitxategia edo baimendu deskarga
# This is the tooltip of the action button shown when uncommon downloads are
# blocked.This opens a dialog where the user can choose whether to open the
# file or remove the download. Opening is the default option.
downloads-cmd-choose-open =
    .tooltiptext = Ireki edo kendu fitxategia
downloads-cmd-choose-open-panel =
    .aria-label = Ireki edo kendu fitxategia
# Displayed when hovering a blocked download, indicates that it's possible to
# show more information for user to take the next action.
downloads-show-more-information =
    .value = Erakutsi informazio gehiago
# Displayed when hovering a complete download, indicates that it's possible to
# open the file using an app available in the system.
downloads-open-file =
    .value = Ireki fitxategia

## Displayed when the user clicked on a download in process. Indicates that the
## downloading file will be opened after certain amount of time using an app
## available in the system.
## Variables:
##   $hours (number) - Amount of hours left till the file opens.
##   $seconds (number) - Amount of seconds left till the file opens.
##   $minutes (number) - Amount of minutes till the file opens.

downloading-file-opens-in-hours-and-minutes = { $hours }h { $minutes }m barru irekitzen???
downloading-file-opens-in-minutes = { $minutes }m barru irekitzen???
downloading-file-opens-in-minutes-and-seconds = { $minutes }m { $seconds }s barru irekitzen???
downloading-file-opens-in-seconds = { $seconds }s barru irekitzen???
downloading-file-opens-in-some-time = Burututakoan irekitzen???

##

# Displayed when hovering a download which is able to be retried by users,
# indicates that it's possible to download this file again.
downloads-retry-download =
    .value = Saiatu berriro deskarga
# Displayed when hovering a download which is able to be cancelled by users,
# indicates that it's possible to cancel and stop the download.
downloads-cancel-download =
    .value = Utzi deskarga
# This string is shown at the bottom of the Downloads Panel when all the
# downloads fit in the available space, or when there are no downloads in
# the panel at all.
downloads-history =
    .label = Erakutsi deskarga guztiak
    .accesskey = s
# This string is shown at the top of the Download Details Panel, to indicate
# that we are showing the details of a single download.
downloads-details =
    .title = Deskargaren xehetasunak
downloads-clear-downloads-button =
    .label = Garbitu deskargak
    .tooltiptext = Burututako, utzitako eta huts egindako deskargak garbitzen ditu
# This string is shown when there are no items in the Downloads view, when it
# is displayed inside a browser tab.
downloads-list-empty =
    .value = Ez dago deskargarik.
# This string is shown when there are no items in the Downloads Panel.
downloads-panel-empty =
    .value = Deskargarik ez saio honetarako.
# This is displayed in an item at the bottom of the Downloads Panel when there
# are more downloads than can fit in the list in the panel.
#   $count (number) - number of files being downloaded that are not shown in the
#                     panel list.
downloads-more-downloading =
    { $count ->
        [one] Fitxategi { $count } gehiago deskargatzen
       *[other] { $count } fitxategi gehiago deskargatzen
    }
