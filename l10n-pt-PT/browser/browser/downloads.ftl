# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## The title and aria-label attributes are used by screen readers to describe
## the Downloads Panel.

downloads-window =
    .title = TransferĂȘncias
downloads-panel =
    .aria-label = TransferĂȘncias

##

# The style attribute has the width of the Downloads Panel expressed using
# a CSS unit. The longest labels that should fit are usually those of
# in-progress and blocked downloads.
downloads-panel-items =
    .style = width: 35em
downloads-cmd-pause =
    .label = Pausar
    .accesskey = P
downloads-cmd-resume =
    .label = Retomar
    .accesskey = R
downloads-cmd-cancel =
    .tooltiptext = Cancelar
downloads-cmd-cancel-panel =
    .aria-label = Cancelar
# This message is only displayed on Windows and Linux devices
downloads-cmd-show-menuitem =
    .label = Abrir pasta de destino
    .accesskey = p
# This message is only displayed on macOS devices
downloads-cmd-show-menuitem-mac =
    .label = Mostrar no Finder
    .accesskey = F
downloads-cmd-show-menuitem-2 =
    .label =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Mostrar na pasta
        }
    .accesskey = M
downloads-cmd-use-system-default =
    .label = Abrir no visualizador do sistema
    .accesskey = v
# We can use the same accesskey as downloads-cmd-always-open-similar-files.
# Both should not be visible in the downloads context menu at the same time.
downloads-cmd-always-use-system-default =
    .label = Abrir sempre no visualizador do sistema
    .accesskey = m
downloads-cmd-show-button =
    .tooltiptext =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Abrir pasta de destino
        }
downloads-cmd-show-panel =
    .aria-label =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Abrir pasta de destino
        }
downloads-cmd-show-description =
    .value =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Abrir pasta de destino
        }
# We can use the same accesskey as downloads-cmd-always-use-system-default.
# Both should not be visible in the downloads context menu at the same time.
downloads-cmd-always-open-similar-files =
    .label = Abrir sempre ficheiros semelhantes
    .accesskey = b
downloads-cmd-show-button-2 =
    .tooltiptext =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Mostrar na pasta
        }
downloads-cmd-show-panel-2 =
    .aria-label =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Mostrar na pasta
        }
downloads-cmd-show-description-2 =
    .value =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Mostrar na pasta
        }
downloads-cmd-show-downloads =
    .label = Mostrar pasta de transferĂȘncias
downloads-cmd-retry =
    .tooltiptext = Voltar a tentar
downloads-cmd-retry-panel =
    .aria-label = Voltar a tentar
downloads-cmd-go-to-download-page =
    .label = Ir para a pĂĄgina da transferĂȘncia
    .accesskey = g
downloads-cmd-copy-download-link =
    .label = Copiar ligaĂ§ĂŁo da transferĂȘncia
    .accesskey = l
downloads-cmd-remove-from-history =
    .label = Remover do histĂłrico
    .accesskey = e
downloads-cmd-clear-list =
    .label = Limpar o painel de prĂ©-visualizaĂ§ĂŁo
    .accesskey = a
downloads-cmd-clear-downloads =
    .label = Limpar transferĂȘncias
    .accesskey = t
# This command is shown in the context menu when downloads are blocked.
downloads-cmd-unblock =
    .label = Permitir transferĂȘncia
    .accesskey = m
# This is the tooltip of the action button shown when malware is blocked.
downloads-cmd-remove-file =
    .tooltiptext = Remover ficheiro
downloads-cmd-remove-file-panel =
    .aria-label = Remover ficheiro
# This is the tooltip of the action button shown when potentially unwanted
# downloads are blocked. This opens a dialog where the user can choose
# whether to unblock or remove the download. Removing is the default option.
downloads-cmd-choose-unblock =
    .tooltiptext = Remover ficheiro ou permitir transferĂȘncia
downloads-cmd-choose-unblock-panel =
    .aria-label = Remover ficheiro ou permitir transferĂȘncia
# This is the tooltip of the action button shown when uncommon downloads are
# blocked.This opens a dialog where the user can choose whether to open the
# file or remove the download. Opening is the default option.
downloads-cmd-choose-open =
    .tooltiptext = Abrir ou remover ficheiro
downloads-cmd-choose-open-panel =
    .aria-label = Abrir ou remover ficheiro
# Displayed when hovering a blocked download, indicates that it's possible to
# show more information for user to take the next action.
downloads-show-more-information =
    .value = Mostrar mais informaĂ§ĂŁo
# Displayed when hovering a complete download, indicates that it's possible to
# open the file using an app available in the system.
downloads-open-file =
    .value = Abrir ficheiro

## Displayed when the user clicked on a download in process. Indicates that the
## downloading file will be opened after certain amount of time using an app
## available in the system.
## Variables:
##   $hours (number) - Amount of hours left till the file opens.
##   $seconds (number) - Amount of seconds left till the file opens.
##   $minutes (number) - Amount of minutes till the file opens.

downloading-file-opens-in-hours-and-minutes = A abrir em { $hours }h { $minutes }mâŠ
downloading-file-opens-in-minutes = A abrir em { $minutes }mâŠ
downloading-file-opens-in-minutes-and-seconds = A abrir em { $minutes }m { $seconds }sâŠ
downloading-file-opens-in-seconds = A abrir em { $seconds }sâŠ
downloading-file-opens-in-some-time = SerĂĄ aberto quando terminarâŠ

##

# Displayed when hovering a download which is able to be retried by users,
# indicates that it's possible to download this file again.
downloads-retry-download =
    .value = RecomeĂ§ar transferĂȘncia
# Displayed when hovering a download which is able to be cancelled by users,
# indicates that it's possible to cancel and stop the download.
downloads-cancel-download =
    .value = Cancelar transferĂȘncia
# This string is shown at the bottom of the Downloads Panel when all the
# downloads fit in the available space, or when there are no downloads in
# the panel at all.
downloads-history =
    .label = Mostrar todas as transferĂȘncias
    .accesskey = s
# This string is shown at the top of the Download Details Panel, to indicate
# that we are showing the details of a single download.
downloads-details =
    .title = Detalhes da transferĂȘncia

## Displayed when a site attempts to automatically download many files.
## Variables:
##   $num (number) - Number of blocked downloads.
##   $url (string) - The url of the suspicious site, stripped of http, https and www prefix.

downloads-files-not-downloaded =
    { $num ->
        [one] Ficheiro nĂŁo transferido
       *[other] { $num } ficheiros nĂŁo transferidos.
    }
downloads-blocked-from-url = TransferĂȘncias de { $url } bloqueadas.
downloads-blocked-download-detailed-info = { $url } tentou transferir automaticamente mĂșltiplos ficheiros. O site pode estar com problemas ou a tentar armazenar ficheiros de spam no seu dispositivo.

##

downloads-clear-downloads-button =
    .label = Limpar transferĂȘncias
    .tooltiptext = Limpa as transferĂȘncias concluĂ­das, canceladas e falhadas
# This string is shown when there are no items in the Downloads view, when it
# is displayed inside a browser tab.
downloads-list-empty =
    .value = NĂŁo existem transferĂȘncias.
# This string is shown when there are no items in the Downloads Panel.
downloads-panel-empty =
    .value = Sem transferĂȘncias para esta sessĂŁo.
# This is displayed in an item at the bottom of the Downloads Panel when there
# are more downloads than can fit in the list in the panel.
#   $count (number) - number of files being downloaded that are not shown in the
#                     panel list.
downloads-more-downloading =
    { $count ->
        [one] EstĂĄ a ser transferido mais { $count } ficheiro
       *[other] EstĂŁo a ser transferidos mais { $count } ficheiros
    }
