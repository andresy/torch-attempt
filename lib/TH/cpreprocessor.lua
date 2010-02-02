local f = io.open(arg[1])
local txt = f:read('*all')
f:close()

function string.strip(str)
   return str:gsub('^%s+', ''):gsub('%s+$', '')
end

txt = '\n' .. txt .. '\n'
txt = txt:gsub('\n%s*#%s*for(.-)\n%s*#%s*endfor%s*\n', function(str)
                                                          local loop = ''
                                                          str = str:gsub('^[^\n]+', function(substr)
                                                                                       loop = loop .. substr
                                                                                       return ''
                                                                                    end)
                                                          
                                                          local nuples = {}
                                                          loop = loop:gsub('%(%s*([_%w%s]+)%s*%)', function(str)
                                                                                                   local nuple = {}

                                                                                                   if not str:match('[_%w%s]+') then
                                                                                                      error('syntax error')
                                                                                                   end

                                                                                                   str = str:gsub('[_%w]+', function(str)
                                                                                                                            table.insert(nuple, str)
                                                                                                                            return ''
                                                                                                                         end)
                                                                                                   table.insert(nuples, nuple)

                                                                                                   if #nuple ~= #(nuples[1]) then
                                                                                                      error('inconsitent nuple sizes')
                                                                                                   end

                                                                                                   return '\021'
                                                                                                end)
                                                          
                                                          local keywords = {}
                                                          if #nuples == 0 then
                                                             loop = loop:gsub('[_%w]+', function(str)
                                                                                        if str == 'in' and #keywords == 1 then
                                                                                           return
                                                                                        else
                                                                                           table.insert(keywords, str)
                                                                                           return '\022'
                                                                                        end
                                                                                     end)
                                                          end

                                                          loop = loop:gsub('%s', '')
                                                          if not loop:match('^\021in\021+$') and not loop:match('^\022in\022+$') then
                                                             error('syntax error')
                                                          end

                                                          local result = ''
                                                          for i=2,#keywords do
                                                             result = result .. string.format('\n#define %s %s\n', keywords[1], keywords[i])
                                                             result = result .. str
                                                             result = result .. string.format('\n#undef %s\n\n', keywords[1])
                                                          end

                                                          for i=2,#nuples do
                                                             result = result .. '\n\n'
                                                             for j=1,#nuples[1] do
                                                                result = result .. string.format('#define %s %s\n', nuples[1][j], nuples[i][j])
                                                             end
                                                             result = result .. str
                                                             result = result .. '\n\n'
                                                             for j=1,#nuples[1] do
                                                                result = result .. string.format('#undef %s\n', nuples[1][j])
                                                             end
                                                             result = result .. '\n'
                                                          end

                                                          return result
                                                       end)

print(txt)
